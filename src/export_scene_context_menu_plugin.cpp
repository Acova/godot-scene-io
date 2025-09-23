#include "export_scene_context_menu_plugin.hpp"

#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/zip_packer.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/templates/hash_set.hpp>

using namespace godot;

void ExportSceneContextMenuPlugin::_bind_methods() {
    ClassDB::bind_method(D_METHOD("export_scene"), &ExportSceneContextMenuPlugin::export_scene);
    ClassDB::bind_method(D_METHOD("_on_file_path_selected"), &ExportSceneContextMenuPlugin::_on_file_path_selected);
}

void _get_dependencies_for_path_recursive(const String &p_path, PackedStringArray &r_collected_paths, PackedStringArray &r_visited_paths) {
    if (
        r_visited_paths.has(p_path)
        || p_path.is_empty()
        || !p_path.begins_with("res://")
    ) {
        // We return if the route was already visited, or if it is not a valid resource route
        return ;
    }

    r_collected_paths.push_back(p_path);
    r_visited_paths.push_back(p_path);

    // First, we include all direct dependencies
    PackedStringArray direct_dependencies = ResourceLoader::get_singleton()->get_dependencies(p_path);
    for (int i = 0; i < direct_dependencies.size(); ++i) {
        String dependency = direct_dependencies[i];
        String dependency_path = dependency.substr(dependency.find("res://"));
        if (!dependency_path.is_empty()) {
            _get_dependencies_for_path_recursive(dependency_path, r_collected_paths, r_visited_paths);
        }
    }

    // TODO: For now, we will only copy the basic dependencies
}

PackedStringArray _get_dependencies_for_path(const String &p_path) {
    PackedStringArray collected_paths;
    PackedStringArray visited_paths;

    _get_dependencies_for_path_recursive(p_path, collected_paths, visited_paths);
    
    return collected_paths;
}

void ExportSceneContextMenuPlugin::export_scene(const PackedStringArray &p_paths) {
    String selected_path = p_paths.get(0);
    String scene_name = selected_path.get_file().trim_suffix(".tscn");
    file_dialog = memnew(EditorFileDialog);
    file_dialog->set_access(EditorFileDialog::ACCESS_FILESYSTEM);
    file_dialog->set_file_mode(EditorFileDialog::FILE_MODE_SAVE_FILE);
    file_dialog->set_filters(PackedStringArray{"*.gpackage ; GPackage files"});
    file_dialog->set_current_file(scene_name + ".gpackage");
    file_dialog->connect("file_selected", Callable(this, "_on_file_path_selected"));
    EditorInterface *editor = EditorInterface::get_singleton();
    editor->get_editor_main_screen()->add_child(file_dialog);
    file_dialog->popup_file_dialog();
}

void ExportSceneContextMenuPlugin::_on_file_path_selected(const String &p_path) {
    print_line("Selected path: " + p_path);
    Ref<ZIPPacker> zip_packer;
    zip_packer.instantiate();
    Error err = zip_packer->open(p_path, ZIPPacker::APPEND_CREATE);
    if (err != Error::OK) {
        print_error("Error intentando abrir el archivo en ZIP");
        return;
    }

    PackedStringArray dependencies = _get_dependencies_for_path(p_path);
    for (int i = 0; i < dependencies.size(); ++i) {
        String file_path = dependencies[i];
        Ref<FileAccess> file_access = FileAccess::open(file_path, FileAccess::READ);
        PackedByteArray file_contents = file_access->get_buffer(file_access->get_length());
        zip_packer->start_file(file_path.substr(String("res://").length(), file_path.length()));
        zip_packer->write_file(file_contents);
        zip_packer->close_file();
        file_access->close();
    }
    
    zip_packer->close();
}

bool should_show_export_option(const PackedStringArray &p_paths) {
    if (p_paths.size() != 1) {
        return false;
    }

    String file_path = p_paths.get(0);
    String extension = file_path.get_extension();
    if (extension != "tscn" && extension != "scn") {
        return false;
    }

    Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(file_path);
    return scene.is_valid();
}

void ExportSceneContextMenuPlugin::_popup_menu(const PackedStringArray &p_paths) {
    if (should_show_export_option(p_paths)) {
        Callable callback = Callable(this, "export_scene");
        add_context_menu_item("Export Scene", callback);
    }
}
#include "export_scene_context_menu_plugin.hpp"

#include <godot_cpp/classes/accept_dialog.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/scene_state.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/zip_packer.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/script.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/templates/hash_set.hpp>

using namespace godot;

void ExportSceneContextMenuPlugin::_bind_methods() {
    ClassDB::bind_method(D_METHOD("export_scene"), &ExportSceneContextMenuPlugin::export_scene);
    ClassDB::bind_method(D_METHOD("_on_file_path_selected"), &ExportSceneContextMenuPlugin::_on_file_path_selected);
}

String _get_base_class_dependency(const String &p_path) {
    if (p_path.get_extension() == "gd") {
        Ref<Script> script = ResourceLoader::get_singleton()->load(p_path);
        if (!script.is_valid()) {
            print_error("Error loading script: " + p_path);
            return "";
        }

        Ref<Script> base_script = script->get_base_script();
        if (!base_script.is_valid()) {
            print_line("No base class for script: " + p_path);
            return "";
        }

        String base_path = base_script->get_path();
        if (base_path.is_empty()) {
            print_line("Base class is built-in for script: " + p_path);
            return "";
        }
        print_line("Base class: " + base_script->get_path());
    }

    if (p_path.get_extension() == "tscn") {
        Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(p_path);
        if (!scene.is_valid()) {
            print_error("Error loading scene: " + p_path);
            return "";
        }

        print_line("Scene class: " + scene->get_class());

        Node *scene_node = scene->instantiate();
        print_line("Node type: " + scene_node->get_class());
    }

    return "";
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
    print_line("Visiting: " + p_path);

    // First, we include all direct dependencies
    PackedStringArray direct_dependencies = ResourceLoader::get_singleton()->get_dependencies(p_path);
    for (int i = 0; i < direct_dependencies.size(); ++i) {
        String dependency = direct_dependencies[i];
        print_line("Direct dependency: " + dependency);
        String dependency_path = dependency.substr(dependency.find("res://"));
        if (!dependency_path.is_empty()) {
            _get_dependencies_for_path_recursive(dependency_path, r_collected_paths, r_visited_paths);
        }
    }

    // TODO: For now, we will only copy the basic dependencies
    if (p_path.get_extension() == "gd") {
        Ref<Script> script = ResourceLoader::get_singleton()->load(p_path);
        if (!script.is_valid()) {
            print_error("Error loading script: " + p_path);
            return;
        }

        Ref<Script> base_script = script->get_base_script();
        if (!base_script.is_valid()) {
            print_error("No base class for script: " + p_path);
            return;
        }

        String base_path = base_script->get_path();
        if (base_path.is_empty()) {
            print_error("Base class is built-in for script: " + p_path);
            return;
        }
        
        print_line("Indirect dependency (base class for script): " + base_path);
        _get_dependencies_for_path_recursive(base_path, r_collected_paths, r_visited_paths);
    }
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
    file_dialog->connect("file_selected", Callable(this, "_on_file_path_selected").bind(selected_path));
    EditorInterface *editor = EditorInterface::get_singleton();
    editor->get_editor_main_screen()->add_child(file_dialog);
    file_dialog->set_current_file(scene_name + ".gpackage");
    file_dialog->popup_file_dialog();
}

void _show_confirmation(const String &exported_path) {
    AcceptDialog *dialog = memnew(AcceptDialog);
    dialog->set_title("Scene Exported");
    dialog->set_text("The scene has been exported to: " + exported_path);
    dialog->get_ok_button()->set_text("OK");
    EditorInterface *editor = EditorInterface::get_singleton();
    editor->get_editor_main_screen()->add_child(dialog);
    dialog->popup_centered();
}

void ExportSceneContextMenuPlugin::_on_file_path_selected(const String &p_path, const String &selected_path) {
    Ref<ZIPPacker> zip_packer;
    zip_packer.instantiate();
    Error err = zip_packer->open(p_path, ZIPPacker::APPEND_CREATE);
    if (err != Error::OK) {
        print_error("Error intentando abrir el archivo en ZIP");
        return;
    }

    PackedStringArray dependencies = _get_dependencies_for_path(selected_path);
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
    _show_confirmation(p_path);
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
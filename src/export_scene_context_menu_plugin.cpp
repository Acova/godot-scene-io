#include "export_scene_context_menu_plugin.hpp"

#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/zip_packer.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/templates/hash_set.hpp>

using namespace godot;

void ExportSceneContextMenuPlugin::_bind_methods() {
    ClassDB::bind_method(D_METHOD("export_scene"), &ExportSceneContextMenuPlugin::export_scene);
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
    // Now, we try to find the more complex dependencies
    // Ref<Resource> resource = ResourceLoader::get_singleton()->load(p_path, "", ResourceLoader::CACHE_MODE_IGNORE);
    // if (resource.is_valid()) {
    //     TypedArray<Dictionary> property_list = resource->get_property_list();
    //     for (int i = 0; i < property_list.size(); ++i) {
    //         Dictionary property = property_list[i];
    //         String property_name = property["name"];
    //         Variant value = resource->get(property_name);
    //         if (value.get_type() == Variant::OBJECT) {
    //             // Add logic to copy these resources
    //         }
    //     }
    // }
}

PackedStringArray _get_dependencies_for_path(const String &p_path) {
    PackedStringArray collected_paths;
    PackedStringArray visited_paths;

    _get_dependencies_for_path_recursive(p_path, collected_paths, visited_paths);
    
    return collected_paths;
}

void ExportSceneContextMenuPlugin::export_scene(const PackedStringArray &p_paths) {
    String selected_path = p_paths.get(0);
    String export_dir = "res://exported_scenes";
    DirAccess::make_dir_recursive_absolute(export_dir);

    String scene_name = selected_path.get_file().trim_suffix(".tscn");
    String export_path = export_dir + "/" + scene_name + ".gpackage";
    print_line("Path del fichero: " + export_path);
    Ref<ZIPPacker> zip_packer;
    zip_packer.instantiate();
    Error err = zip_packer->open(export_path, ZIPPacker::APPEND_CREATE);
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
}

void ExportSceneContextMenuPlugin::_popup_menu(const PackedStringArray &p_paths) {
    if (should_show_export_option(p_paths)) {
        Callable callback = Callable(this, "export_scene");
        add_context_menu_item("Export Scene", callback);
    }
}
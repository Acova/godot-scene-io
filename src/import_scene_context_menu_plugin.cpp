#include "import_scene_context_menu_plugin.hpp"

#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/editor_file_system.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/zip_reader.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void ImportSceneContextMenuPlugin::_bind_methods() {
    ClassDB::bind_method(D_METHOD("import_scene"), &ImportSceneContextMenuPlugin::import_scene);
}

void ImportSceneContextMenuPlugin::import_scene(const PackedStringArray &p_paths) {
    String selected_path = p_paths[0];
    String import_dir = "res://";

    Ref<ZIPReader> zip_reader;
    zip_reader.instantiate();
    Error err = zip_reader->open(selected_path);
    if (err != Error::OK) {
        print_error("Error intentando abrir el archivo GPACKAGE");
        return;
    }

    Ref<DirAccess> dir_access = DirAccess::open(import_dir);
    PackedStringArray file_list = zip_reader->get_files();
    for (int i = 0; i < file_list.size(); ++i) {
        String file_path = file_list[i];
        String export_path = dir_access->get_current_dir().path_join(file_path);
    
        // We create the folder to extract the file, if neccessary
        dir_access->make_dir_recursive(export_path.get_base_dir());

        // We create the file and export the contents
        Ref<FileAccess> file = FileAccess::open(export_path, FileAccess::WRITE);
        PackedByteArray buffer = zip_reader->read_file(file_path);
        file->store_buffer(buffer);
    }

    zip_reader->close();
    EditorInterface::get_singleton()->get_resource_filesystem()->scan();
}

bool should_show_import_option(const PackedStringArray &p_paths) {
    if (p_paths.size() != 1) {
        return false;
    }

    String file_path = p_paths.get(0);
    String extension = file_path.get_extension();
    if (extension != "gpackage") {
        return false;
    }

    return true;
}

void ImportSceneContextMenuPlugin::_popup_menu(const PackedStringArray &p_paths) {
    if (should_show_import_option(p_paths)) {
        Callable callback = Callable(this, "import_scene");
        add_context_menu_item("Import Scene", callback);
    }
}
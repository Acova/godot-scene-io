#include "export_scene_context_menu_plugin.hpp"

#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/zip_packer.hpp>
#include <godot_cpp/core/class_db.hpp>

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

void ExportSceneContextMenuPlugin::export_scene(const PackedStringArray &p_paths) {
    print_line("Exportando escena...");
    String selected_path = p_paths.get(0);
    String export_dir = "res://exported_scenes";
    DirAccess::make_dir_recursive_absolute(export_dir);

    print_line("Creando el fichero zip");
    Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(selected_path);
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

    print_line("Leyendo los contenidos de la escena");
    Ref<FileAccess> file_access = FileAccess::open(selected_path, FileAccess::READ);
    PackedByteArray file_contents = file_access->get_buffer(file_access->get_length());
    zip_packer->start_file(selected_path.get_file());
    zip_packer->write_file(file_contents);
    
    print_line("Cerrando los ficheros");
    zip_packer->close_file();
    zip_packer->close();
    file_access->close();
}

void ExportSceneContextMenuPlugin::_popup_menu(const PackedStringArray &p_paths) {
    if (should_show_export_option(p_paths)) {
        Callable callback = Callable(this, "export_scene");
        add_context_menu_item("Export Scene", callback);
    }
}
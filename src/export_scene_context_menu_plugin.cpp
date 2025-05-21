#include "export_scene_context_menu_plugin.hpp"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
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
}

void ExportSceneContextMenuPlugin::_popup_menu(const PackedStringArray &p_paths) {
    if (should_show_export_option(p_paths)) {
        Callable callback = Callable(this, "export_scene");
        add_context_menu_item("Export Scene", callback);
    }
}
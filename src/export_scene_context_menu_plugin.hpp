#ifndef EXPORT_SCENE_CONTEXT_MENU_PLUGIN_H
#define EXPORT_SCENE_CONTEXT_MENU_PLUGIN_H

#include <godot_cpp/classes/editor_context_menu_plugin.hpp>

using namespace godot;

class ExportSceneContextMenuPlugin : public EditorContextMenuPlugin {
    GDCLASS(ExportSceneContextMenuPlugin, EditorContextMenuPlugin)

protected:
    static void _bind_methods();

public:
    ExportSceneContextMenuPlugin() {};
    ~ExportSceneContextMenuPlugin() {};

    virtual void _popup_menu(const PackedStringArray &p_paths) override;

    void export_scene(const PackedStringArray &p_paths);
};

#endif
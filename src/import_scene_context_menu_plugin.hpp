#ifndef IMPORT_SCENE_CONTEXT_MENU_PLUGIN_H
#define IMPORT_SCENE_CONTEXT_MENU_PLUGIN_H

#include <godot_cpp/classes/editor_context_menu_plugin.hpp>

using namespace godot;

class ImportSceneContextMenuPlugin : public EditorContextMenuPlugin {
    GDCLASS(ImportSceneContextMenuPlugin, EditorContextMenuPlugin);

protected:
    static void _bind_methods();

public:
    ImportSceneContextMenuPlugin() {};
    ~ImportSceneContextMenuPlugin() {};

    virtual void _popup_menu(const PackedStringArray &p_paths) override;

    void import_scene(const PackedStringArray &p_paths);
};

#endif
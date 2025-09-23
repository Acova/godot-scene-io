#ifndef EXPORT_SCENE_CONTEXT_MENU_PLUGIN_H
#define EXPORT_SCENE_CONTEXT_MENU_PLUGIN_H

#include <godot_cpp/classes/editor_context_menu_plugin.hpp>
#include <godot_cpp/classes/editor_file_dialog.hpp>

using namespace godot;

class ExportSceneContextMenuPlugin : public EditorContextMenuPlugin {
    GDCLASS(ExportSceneContextMenuPlugin, EditorContextMenuPlugin)

protected:
    static void _bind_methods();
    EditorFileDialog *file_dialog;

public:
    ExportSceneContextMenuPlugin() {};
    ~ExportSceneContextMenuPlugin() {};

    virtual void _popup_menu(const PackedStringArray &p_paths) override;

    void export_scene(const PackedStringArray &p_paths);
    void _on_file_path_selected(const String &p_path, const String &exported_path);
};

#endif
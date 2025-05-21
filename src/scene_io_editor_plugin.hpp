#ifndef SCENE_IO_EDITOR_PLUGIN_H
#define SCENE_IO_EDITOR_PLUGIN_H

#include "export_scene_context_menu_plugin.hpp"

#include <godot_cpp/classes/editor_plugin.hpp>

namespace godot {

class SceneIOEditorPlugin : public EditorPlugin {
	GDCLASS(SceneIOEditorPlugin, EditorPlugin)

private:
	Ref<ExportSceneContextMenuPlugin> export_scene_context_menu_plugin;

protected:
	static void _bind_methods() {};

public:
	SceneIOEditorPlugin();
	~SceneIOEditorPlugin();

    void _enter_tree() override;
    void _exit_tree() override;
};

}

#endif
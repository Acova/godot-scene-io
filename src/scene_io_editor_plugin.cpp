#include "scene_io_editor_plugin.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void SceneIOEditorPlugin::_enter_tree() {
    export_scene_context_menu_plugin.instantiate();
    add_context_menu_plugin(EditorContextMenuPlugin::CONTEXT_SLOT_FILESYSTEM, export_scene_context_menu_plugin);
}

void SceneIOEditorPlugin::_exit_tree() {
    remove_context_menu_plugin(export_scene_context_menu_plugin);
}

SceneIOEditorPlugin::SceneIOEditorPlugin() {
	// Initialize any variables here.
}

SceneIOEditorPlugin::~SceneIOEditorPlugin() {
	// Add your cleanup here.
}
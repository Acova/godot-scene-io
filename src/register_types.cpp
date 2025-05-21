#include "register_types.h"

#include "export_scene_context_menu_plugin.hpp"
#include "exported_scene_resource.hpp"
#include "exported_scene_resource_format_loader.hpp"
#include "scene_io_editor_plugin.hpp"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/editor_plugin_registration.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

Ref<ExportedSceneResourceFormatLoader> exported_scene_resource_format_loader;

void initialize_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_EDITOR) {
		return;
	}

	GDREGISTER_CLASS(ExportedSceneResource);
	GDREGISTER_CLASS(ExportedSceneResourceFormatLoader);
	exported_scene_resource_format_loader.instantiate();
	ResourceLoader::get_singleton()->add_resource_format_loader(exported_scene_resource_format_loader);

	GDREGISTER_CLASS(ExportSceneContextMenuPlugin);
	GDREGISTER_CLASS(SceneIOEditorPlugin);
	EditorPlugins::add_by_type<SceneIOEditorPlugin>();
}

void uninitialize_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_EDITOR) {
		return;
	}
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT scene_io_plugin_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_module);
	init_obj.register_terminator(uninitialize_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_EDITOR);

	return init_obj.init();
}
}
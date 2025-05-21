#include "exported_scene_resource_format_loader.hpp"

using namespace godot;

PackedStringArray ExportedSceneResourceFormatLoader::_get_recognized_extensions() const {
	PackedStringArray extensions;
	extensions.push_back("gpackage");
	return extensions;
}

bool ExportedSceneResourceFormatLoader::_recognize_path(const String &p_path, const StringName &p_type) const {
	return p_path.ends_with(".gpackage");
}

bool ExportedSceneResourceFormatLoader::_handles_type(const StringName &p_type) const {
	return p_type == String("ExportedSceneResource");
}

String ExportedSceneResourceFormatLoader::_get_resource_type(const String &p_path) const {
	if (p_path.ends_with(".gpackage")) {
		return "ExportedSceneResource";
	}

	return "";
}

Variant ExportedSceneResourceFormatLoader::_load(const String &p_path, const String &p_original_path, bool p_use_sub_threads, int32_t p_cache_mode) const {
	return OK;
}
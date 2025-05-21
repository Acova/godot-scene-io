#ifndef EXPORTED_SCENE_RESOURCE_FORMAT_LOADER
#define EXPORTED_SCENE_RESOURCE_FORMAT_LOADER

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_format_loader.hpp>

using namespace godot;

class ExportedSceneResourceFormatLoader : public ResourceFormatLoader {
    GDCLASS(ExportedSceneResourceFormatLoader, ResourceFormatLoader);

public:
    virtual PackedStringArray _get_recognized_extensions() const override;
	virtual bool _recognize_path(const String &p_path, const StringName &p_type) const override;
	virtual bool _handles_type(const StringName &p_type) const override;
	virtual String _get_resource_type(const String &p_path) const override;
	virtual Variant _load(const String &p_path, const String &p_original_path, bool p_use_sub_threads, int32_t p_cache_mode) const override;

protected:
    static void _bind_methods() {};
};

#endif
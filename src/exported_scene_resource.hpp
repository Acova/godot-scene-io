#ifndef EXPORTED_SCENE_RESOURCE_H
#define EXPORTED_SCENE_RESOURCE_H

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class ExportedSceneResource : public Resource {
    GDCLASS(ExportedSceneResource, Resource);

protected:
    static void _bind_methods();

public:
    ExportedSceneResource() {};
    ~ExportedSceneResource() {};

    void set_custom_data(const String &p_data);
    String get_custom_data() const;
};

#endif
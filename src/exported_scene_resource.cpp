#include "exported_scene_resource.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void ExportedSceneResource::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_custom_data", "data"), &ExportedSceneResource::set_custom_data);
    ClassDB::bind_method(D_METHOD("get_custom_data"), &ExportedSceneResource::get_custom_data);
}

void ExportedSceneResource::set_custom_data(const String &p_data) {
    emit_changed();
}

String ExportedSceneResource::get_custom_data() const {
    return "";
}
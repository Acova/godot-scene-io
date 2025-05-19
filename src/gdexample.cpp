#include "gdexample.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void GDExample::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_export_scene"), &GDExample::_export_scene);
}

void GDExample::_enter_tree() {
    Callable method = Callable(this, "_export_scene");
    add_tool_menu_item("Export Scene", method);
}

void GDExample::_exit_tree() {
    remove_tool_menu_item("Export Scene");
}

void GDExample::_export_scene(const String &scene_path) {
    print_line("Exportando escena...");
}

GDExample::GDExample() {
	// Initialize any variables here.
}

GDExample::~GDExample() {
	// Add your cleanup here.
}
#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <godot_cpp/classes/editor_plugin.hpp>

namespace godot {

class GDExample : public EditorPlugin {
	GDCLASS(GDExample, EditorPlugin)

private:
    void _export_scene(const String &scene_path);

protected:
	static void _bind_methods();

public:
	GDExample();
	~GDExample();

    void _enter_tree() override;
    void _exit_tree() override;
};

}

#endif
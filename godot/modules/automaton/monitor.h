
#ifndef GODOT_MONITOR_H
#define GODOT_MONITOR_H

#include "core/hash_map.h"
#include "core/reference.h"
#include "main/performance.h"
#include "scene/gui/control.h"
#include "scene/gui/label.h"
#include "scene/gui/split_container.h"

class Monitor : public Control {
	GDCLASS(Monitor, Control);

private:	
	int marker_frame = 0;
	const int MARGIN = 4;
	const int POINT_SEPARATION = 5;
	const int MARKER_MARGIN = 2;
	int my_test_prop = 0;

	static String _create_label(float p_value, Performance::MonitorType p_type);
	// void _monitor_select();
	void _draw();
	void _gui_input(const Ref<InputEvent> &p_event);

protected:
	void _notification(int p_notification);
    static void _bind_methods();

public:
	String name;
	// String base;
	List<float> history;
	float max = 0.0f;
	Performance::MonitorType type = Performance::MONITOR_TYPE_QUANTITY;
	// int frame_index = 0;

	Monitor();
	
	void update_value(float p_value);
	void set_name(String p_name);
	void set_my_prop(int p_val) { my_test_prop = p_val; }
	void reset();
};

#endif // GODOT_MONITOR_H

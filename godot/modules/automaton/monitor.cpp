#include "monitor.h"

#include "core/math/random_number_generator.h"
#include "editor/editor_scale.h"
#include "editor/editor_settings.h"
#include "main/performance.h"
#include "scene/2d/canvas_item.h"

Monitor::Monitor() {
	name = "Monitor";
	// set_clip_contents(true);
	
	set_mouse_filter(MOUSE_FILTER_STOP);

	// connect("draw", this, "_draw");
	// connect("gui_input", this, "_gui_input");
}

void Monitor::_notification(int p_what) {
//	static float d_time = 0;
	if (NOTIFICATION_DRAW == p_what) {
		/*
		d_time += 0.01;
		update_value(Math::randf() * 100.0);
		d_time += 0.01;
		update_value(Math::randf() * 95.0);
		d_time += 0.01;
		update_value(Math::randf() * 90.0);
		d_time += 0.01;
		update_value(Math::randf() * 85.0);
		d_time += 0.01;
		update_value(Math::randf() * 80.0);
		d_time += 0.01;
		update_value(Math::randf() * 80.0);
		d_time += 0.01;
		update_value(Math::randf() * 80.0);
		d_time += 0.01;
		update_value(Math::randf() * 85.0);
		d_time += 0.01;
		update_value(Math::randf() * 90.0);
		d_time += 0.01;
		update_value(Math::randf() * 95.0);
		d_time += 0.01;
		update_value(Math::randf() * 100.0);
		*/
		
		_draw();
	}
}

void Monitor::_bind_methods() {
    // ClassDB::bind_method(D_METHOD("set_name", "name"), &Monitor::set_name);
	ClassDB::bind_method(D_METHOD("set_my_prop", "val"), &Monitor::set_my_prop);
	// ClassDB::bind_method(D_METHOD("_draw"), &Monitor::_draw);
	ClassDB::bind_method(D_METHOD("_gui_input"), &Monitor::_gui_input);
}

void Monitor::set_name(String p_name) {
	name = p_name;
	update();
}

void Monitor::update_value(float p_value) {
	history.push_front(p_value);
	if (p_value > max) {
		max = p_value;
	}

	marker_frame++;
	update();
}

void Monitor::reset() {
	history.clear();
	max = 0.0f;
	update();
}

String Monitor::_create_label(float p_value, Performance::MonitorType p_type) {
	return String::humanize_size(p_value);
	// switch (p_type) {
	// 	case Performance::MONITOR_TYPE_MEMORY: {
	// 		return String::humanize_size(p_value);
	// 	}
	// 	case Performance::MONITOR_TYPE_TIME: {
	// 		return TS->format_number(rtos(p_value * 1000).pad_decimals(2)) + " " + RTR("ms");
	// 	}
	// 	default: {
	// 		return TS->format_number(rtos(p_value));
	// 	}
	// }
}

// void Monitor::_monitor_select() {
// 	queue_redraw();
// }

void Monitor::_draw() {
	
	// Ref<StyleBox> graph_style_box = get_theme_stylebox(StringName("normal"), StringName("TextEdit"));
	// Ref<Font> graph_font = get_font(StringName("font"), StringName("TextEdit"));
	// int font_size = get_theme_font_size(StringName("font_size"), StringName("TextEdit"));
	Ref<StyleBox> graph_style_box = get_stylebox(StringName("normal"), StringName("TextEdit"));
	Ref<Font> graph_font = get_font(StringName("font"), StringName("TextEdit"));

	Size2i cell_size = Size2i(get_size());
	// float spacing = float(POINT_SEPARATION) / float(columns);
	float spacing = 5.0f;
	float value_multiplier = 1.4f; // EditorSettings::get_singleton()->is_dark_theme() ? 1.4f : 0.55f;
	// float hue_shift = 1.0f / float(monitors.size());
	float hue_shift = 1.0f;

	// Monitor &current = monitors[active[i]];
	Rect2i rect(Point2i(MARGIN, MARGIN), cell_size - Point2i(MARGIN, MARGIN) * 2);
	draw_style_box(graph_style_box, rect);

	rect.position += graph_style_box->get_offset();
	rect.size -= graph_style_box->get_minimum_size();
	Color draw_color = get_color(StringName("accent_color"), StringName("Editor"));
	draw_color.set_hsv(Math::fmod(hue_shift, 0.9f), draw_color.get_s() * 0.9f, draw_color.get_v() * value_multiplier, 0.6f);
	draw_string(graph_font, rect.position + Point2(0, graph_font->get_ascent()), name, draw_color);

	draw_color.a = 0.9f;
	float value_position = rect.size.width - graph_font->get_string_size(name).width;
	if (value_position < 0) {
		value_position = 0;
	}
	draw_string(graph_font, rect.position + Point2(value_position, graph_font->get_ascent()), name, draw_color);

	rect.position.y += graph_font->get_height();
	rect.size.height -= graph_font->get_height();

	// testing ...
//	draw_color = Color(1,0,0,1);
	
	/*
	int line_count = rect.size.height / (graph_font->get_height(font_size) * 2);
	if (line_count > 5) {
		line_count = 5;
	}
	if (line_count > 0) {
		Color horizontal_line_color;
		horizontal_line_color.set_hsv(draw_color.get_h(), draw_color.get_s() * 0.5f, draw_color.get_v() * 0.5f, 0.3f);
		draw_line(rect.position, rect.position + Vector2(rect.size.width, 0), horizontal_line_color, Math::round(EDSCALE));
		draw_string(graph_font, rect.position + Vector2(0, graph_font->get_ascent(font_size)), _create_label(max, type), HORIZONTAL_ALIGNMENT_LEFT, rect.size.width, font_size, horizontal_line_color);

		for (int j = 0; j < line_count; j++) {
			Vector2 y_offset = Vector2(0, rect.size.height * (1.0f - float(j) / float(line_count)));
			draw_line(rect.position + y_offset, rect.position + Vector2(rect.size.width, 0) + y_offset, horizontal_line_color, Math::round(EDSCALE));
			draw_string(graph_font, rect.position - Vector2(0, graph_font->get_descent(font_size)) + y_offset, _create_label(max * float(j) / float(line_count), type), HORIZONTAL_ALIGNMENT_LEFT, rect.size.width, font_size, horizontal_line_color);
		}
	}
	 */
	
	float from = rect.size.width;
	float prev = -1.0f;
	int count = 0;
	List<float>::Element *e = history.front();

	while (from >= 0 && e) {
		float m = max;
		float h2 = 0;
		if (m != 0) {
			h2 = (e->get() / m);
		}
		h2 = (1.0f - h2) * float(rect.size.y);
		if (e != history.front()) {
			draw_line(rect.position + Point2(from, h2), rect.position + Point2(from + spacing, prev), draw_color, Math::round(EDSCALE));
		}

		//if (marker_key == active[i] && count == marker_frame) {
		if (count == marker_frame) {
			Color line_color;
			line_color.set_hsv(draw_color.get_h(), draw_color.get_s() * 0.8f, draw_color.get_v(), 0.5f);
			draw_line(rect.position + Point2(from, 0), rect.position + Point2(from, rect.size.y), line_color, Math::round(EDSCALE));

			String label = _create_label(e->get(), type);
			Size2 size = graph_font->get_string_size(label);
			Vector2 text_top_left_position = Vector2(from, h2) - (size + Vector2(MARKER_MARGIN, MARKER_MARGIN));
			if (text_top_left_position.x < 0) {
				text_top_left_position.x = from + MARKER_MARGIN;
			}
			if (text_top_left_position.y < 0) {
				text_top_left_position.y = h2 + MARKER_MARGIN;
			}
			draw_string(graph_font, rect.position + text_top_left_position + Point2(0, graph_font->get_ascent()), label, line_color);
		}
		prev = h2;
		e = e->next();
		from -= spacing;
		count++;
	}
}

/*
void EditorPerformanceProfiler::_build_monitor_tree() {
	HashSet<StringName> monitor_checked;
	for (KeyValue<StringName, Monitor> &E : monitors) {
		if (E.value.item && E.value.item->is_checked(0)) {
			monitor_checked.insert(E.key);
		}
	}

	base_map.clear();
	monitor_tree->get_root()->clear_children();

	for (KeyValue<StringName, Monitor> &E : monitors) {
		TreeItem *base = _get_monitor_base(E.value.base);
		TreeItem *item = _create_monitor_item(E.value.name, base);
		item->set_checked(0, monitor_checked.has(E.key));
		E.value.item = item;
		if (!E.value.history.is_empty()) {
			E.value.update_value(E.value.history.front()->get());
		}
	}
}


TreeItem *EditorPerformanceProfiler::_get_monitor_base(const StringName &p_base_name) {
	if (base_map.has(p_base_name)) {
		return base_map[p_base_name];
	}

	TreeItem *base = monitor_tree->create_item(monitor_tree->get_root());
	base->set_text(0, p_base_name);
	base->set_editable(0, false);
	base->set_selectable(0, false);
	base->set_expand_right(0, true);
	base->set_custom_font(0, get_theme_font(SNAME("bold"), SNAME("EditorFonts")));
	base_map.insert(p_base_name, base);
	return base;
}

TreeItem *EditorPerformanceProfiler::_create_monitor_item(const StringName &p_monitor_name, TreeItem *p_base) {
	TreeItem *item = monitor_tree->create_item(p_base);
	item->set_cell_mode(0, TreeItem::CELL_MODE_CHECK);
	item->set_editable(0, true);
	item->set_selectable(0, false);
	item->set_selectable(1, false);
	item->set_text(0, p_monitor_name);
	return item;
}
*/

void Monitor::_gui_input(const Ref<InputEvent> &p_event) {
	Ref<InputEventMouseButton> mb = p_event;
	if (mb.is_valid() && mb->is_pressed() && mb->get_button_index() == BUTTON_LEFT) {
		// testing...
		update_value((float)mb->get_position().y);
		//
		
		Size2i cell_size = Size2i(get_size());
		Rect2i rect(cell_size + Point2i(MARGIN, MARGIN), cell_size - Point2i(MARGIN, MARGIN) * 2);
		if (rect.has_point(mb->get_position())) {
			// marker_key = name;
			Ref<StyleBox> graph_style_box = get_stylebox(StringName("normal"), StringName("TextEdit"));
			rect.position += graph_style_box->get_offset();
			rect.size -= graph_style_box->get_minimum_size();
			Vector2 point = mb->get_position() - rect.position;
			if (point.x >= rect.size.x) {
				marker_frame = 0;
			} else {
				int point_sep = 5;
				float spacing = float(point_sep);
				marker_frame = (rect.size.x - point.x) / spacing;
			}
			update();
			return;
		}
		
		// marker_key = "";
		update();
	}
}
/*
void EditorPerformanceProfiler::update_monitors(const Vector<StringName> &p_names) {
	
	HashMap<StringName, int> names;
	for (int i = 0; i < p_names.size(); i++) {
		names.insert("custom:" + p_names[i], Performance::MONITOR_MAX + i);
	}

	{
		HashMap<StringName, Monitor>::Iterator E = monitors.begin();
		while (E != monitors.end()) {
			HashMap<StringName, Monitor>::Iterator N = E;
			++N;
			if (String(E->key).begins_with("custom:")) {
				if (!names.has(E->key)) {
					monitors.remove(E);
				} else {
					E->value.frame_index = names[E->key];
					names.erase(E->key);
				}
			}
			E = N;
		}
	}

	for (const KeyValue<StringName, int> &E : names) {
		String name = String(E.key).replace_first("custom:", "");
		String base = "Custom";
		if (name.get_slice_count("/") == 2) {
			base = name.get_slicec('/', 0);
			name = name.get_slicec('/', 1);
		}
		monitors.insert(E.key, Monitor(name, base, E.value, Performance::MONITOR_TYPE_QUANTITY, nullptr));
	}

	_build_monitor_tree();	
}
*/

// TODO: might be needed but not sure why? Could be rewritten to work for just one monitor
// void EditorPerformanceProfiler::add_profile_frame(const Vector<float> &p_values) {
// 	for (KeyValue<StringName, Monitor> &E : monitors) {
// 		float value = 0.0f;
// 		if (E.value.frame_index >= 0 && E.value.frame_index < p_values.size()) {
// 			value = p_values[E.value.frame_index];
// 		}
// 		E.value.history.push_front(value);
// 		E.value.update_value(value);
// 	}
// 	marker_frame++;
// 	monitor_draw->queue_redraw();
// }

// TODO: same comment as above
// List<float> *EditorPerformanceProfiler::get_monitor_data(const StringName &p_name) {
// 	if (monitors.has(p_name)) {
// 		return &monitors[p_name].history;
// 	}
// 	return nullptr;
// }

/*
EditorPerformanceProfiler::EditorPerformanceProfiler() {
	set_name(TTR("Monitors"));
	set_split_offset(340 * EDSCALE);

	monitor_tree = memnew(Tree);
	monitor_tree->set_columns(2);
	monitor_tree->set_column_title(0, TTR("Monitor"));
	monitor_tree->set_column_title(1, TTR("Value"));
	monitor_tree->set_column_titles_visible(true);
	monitor_tree->connect("item_edited", callable_mp(this, &EditorPerformanceProfiler::_monitor_select));
	monitor_tree->create_item();
	monitor_tree->set_hide_root(true);
	add_child(monitor_tree);

	monitor_draw = memnew(Control);
	monitor_draw->set_clip_contents(true);
	monitor_draw->connect("draw", callable_mp(this, &EditorPerformanceProfiler::_monitor_draw));
	monitor_draw->connect("gui_input", callable_mp(this, &EditorPerformanceProfiler::_marker_input));
	add_child(monitor_draw);

	info_message = memnew(Label);
	info_message->set_text(TTR("Pick one or more items from the list to display the graph."));
	info_message->set_vertical_alignment(VERTICAL_ALIGNMENT_CENTER);
	info_message->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
	info_message->set_autowrap_mode(TextServer::AUTOWRAP_WORD_SMART);
	info_message->set_custom_minimum_size(Size2(100 * EDSCALE, 0));
	info_message->set_anchors_and_offsets_preset(PRESET_FULL_RECT, PRESET_MODE_KEEP_SIZE, 8 * EDSCALE);
	monitor_draw->add_child(info_message);

	for (int i = 0; i < Performance::MONITOR_MAX; i++) {
		String base = EditorPropertyNameProcessor::get_singleton()->process_name(Performance::get_singleton()->get_monitor_name(Performance::Monitor(i)).get_slicec('/', 0), EditorPropertyNameProcessor::STYLE_CAPITALIZED);
		String name = EditorPropertyNameProcessor::get_singleton()->process_name(Performance::get_singleton()->get_monitor_name(Performance::Monitor(i)).get_slicec('/', 1), EditorPropertyNameProcessor::STYLE_CAPITALIZED);
		monitors.insert(Performance::get_singleton()->get_monitor_name(Performance::Monitor(i)), Monitor(name, base, i, Performance::get_singleton()->get_monitor_type(Performance::Monitor(i)), nullptr));
	}

	_build_monitor_tree();
}
*/

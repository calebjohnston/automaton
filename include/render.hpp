//
//  render.hpp
//
//  Created by Caleb Johnston on 7/29/2023.
//  Copyright © 2023 Caleb Johnston. All rights reserved.
//

#pragma once

#include <memory>
#include <string>

#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/loop.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/util/ref.hpp"

#include "model.hpp"

namespace Auto {

void render(ResultSet& results)
{
	std::for_each(results.cbegin(), results.cend(), [](const Result& r){ std::cout << r.status << " " << r.message << std::endl; });
}

void render_1(ResultSet& results)
{
	std::string cmd_str;
	std::string output_str;
	
	auto input_option = ftxui::InputOption();
	input_option.on_enter = [&] {
		output_str = cmd_str;
		cmd_str = "";
	};
	auto input_command = ftxui::Input(&cmd_str, "_", input_option);
	auto component = ftxui::Container::Vertical({ input_command });
	
	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({
			ftxui::text(output_str),
			ftxui::separator(),
			input_command->Render(),
		}) | ftxui::border;
	});
	
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	screen.Loop(renderer);
	
//	ftxui::Loop loop(&screen, component);
//	loop.RunOnce();
}

}

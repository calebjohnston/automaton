#include <memory>
#include <string>
#include <vector>

#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include "model.h"
#include "game.h"

namespace Auto {

ftxui::Component CommandPalette(ftxui::StringRef cmd, std::function<void(std::string)> submit)
{
	class Impl : public ftxui::ComponentBase {
	public:
		Impl(ftxui::StringRef cmd, std::function<void(std::string)> on_submit)
		: cmd_(cmd), on_submit_(std::move(on_submit))
		{
			using namespace ftxui;
			
			depth = 0;
			
//			Agent* player = &the_game.agents[0];
//			program_names(player->kernel);
			
			command = "";
			function = "";
			argument = "";
			command_index = 0;
			function_index = 0;
			argument_index = 0;
			command_focus = 0;
			function_focus = 0;
			argument_focus = 0;
			commands = {
				"ps", "fs", "dev", "net", "conn", "msg"
			};
			arguments = {
				"whatever", "dude"
			};
			ps_functions = { "list", "info", "install", "uninstall" };
			fs_functions = { "list", "info", "copy", "delete" };
			dev_functions = { "list", "info", "config", "restore" };
			net_functions = { "scan", "whois", "route" };
			conn_functions = { "wintermute", "case", "armitage" };
			msg_functions = { "ping", "xfer", "cfg", "simplex", "inf" };
			functions = {
				ps_functions, fs_functions, dev_functions, net_functions, conn_functions, msg_functions
			};
			fn_options = functions[0];
			
			// Depth = 0
			MenuOption menu_0_option;
			menu_0_selection = [&] {
				command = commands[command_index];
				fn_options = functions[command_index];
				
				function_focus = argument_focus = 0;
				function_index = argument_index = 0;
				function = argument = "";
				cmd_ = command;
				
				depth = 1;
				
				BuildDependentMenus();
			};
			menu_0_option.on_select = menu_0_selection;
			menu_0_option.focused_entry = &command_focus;
			depth_0_container = Menu(&commands, &command_index, menu_0_option);
			depth_0_renderer = Renderer(depth_0_container, [&] {
				return hbox({
					vbox(depth_0_container->Render()) | size(WIDTH, EQUAL, 15),
				});
			});
			
			BuildDependentMenus();
		}
		
		void BuildDependentMenus() {
			using namespace ftxui;
			
			DetachAllChildren();
			
			// Depth = 1
			MenuOption menu_1_option;
			menu_1_selection = [&] {
				function = fn_options[function_index];
				
				argument_focus = 0;
				argument_index = 0;
				argument = "";
				
				if (4 == command_index) {
					depth = 0;
					on_submit_(command + " " + function);
				} else {
					depth = 2;
					cmd_ = command + " " + function;
				}
			};
			menu_1_option.on_select = menu_1_selection;
			menu_1_option.focused_entry = &function_focus;
			depth_1_container = Menu(&functions[command_index], &function_index, menu_1_option);
			depth_1_renderer = Renderer(depth_1_container, [&] {
				return hbox({
					vbox(depth_1_container->Render()) | size(WIDTH, GREATER_THAN, 15),
				});
			});
			
			// Depth = 2
			MenuOption menu_2_option;
			menu_2_selection = [&] {
				argument = arguments[argument_index];
				depth = 0;
				cmd_ = command + " " + function + " " + argument;
				on_submit_(command + " " + function + " " + argument);
			};
			menu_2_option.on_select = menu_2_selection;
			menu_2_option.focused_entry = &argument_focus;
			depth_2_container = Menu(&arguments, &argument_index, menu_2_option);
			depth_2_renderer = Renderer(depth_2_container, [&] {
				return hbox({
					vbox(depth_2_container->Render()) | size(WIDTH, GREATER_THAN, 12),
				});
			});
			
			// Menu system
			main_container = Container::Tab({
				depth_0_renderer,
				depth_1_renderer,
				depth_2_renderer,
			}, &depth);
			
			Add(main_container);
		}
		
		ftxui::Element Render() override {
			using namespace ftxui;
		
			if (depth == 0) {
				return vbox({
					filler() | yflex_grow,
					hbox({
						window(text(" Command: "), hbox({
							depth_0_renderer->Render(),
						})) | clear_under,
						filler() | xflex_grow,
					}),
					filler() | size(HEIGHT, EQUAL, 1)
				});
			}
			else  if (depth == 1) {
				return vbox({
					filler() | yflex_grow,
					hbox({
						window(text(" Command: "), hbox({
							depth_0_renderer->Render(),
							separator(),
							depth_1_renderer->Render(),
						})) | clear_under,
						filler() | xflex_grow
					}),
					filler() | size(HEIGHT, EQUAL, 1)
				});
			}
			else if (depth == 2) {
				return vbox({
					filler() | yflex_grow,
					hbox({
						window(text(" Command: "), hbox({
							depth_0_renderer->Render(),
							separator(),
							depth_1_renderer->Render(),
							separator(),
							depth_2_renderer->Render(),
						})) | clear_under,
						filler() | xflex_grow
					}),
					filler() | size(HEIGHT, EQUAL, 1)
				});
			}
			else {
				return hbox();
			}
		}
		
		bool OnEvent(ftxui::Event event) override {
			if (event == ftxui::Event::Escape || event == ftxui::Event::ArrowLeft) {
				depth--;
				if (depth < 0) {
					depth = 0;
					command = function = argument = "";
					command_index = function_index = argument_index = 0;
					on_submit_(command);
				}
				return true;
			}
			else if (event == ftxui::Event::ArrowRight) {
				switch (depth) {
					case 0: menu_0_selection(); break;
					case 1: menu_1_selection(); break;
					case 2: menu_2_selection(); break;
					default: return false;
				}
				return true;
			}
			else if (event.is_mouse()) {
				return ComponentBase::OnEvent(event);
			}
			else {
				return ComponentBase::OnEvent(event);
			}
		}
		
		bool Focusable() const final { return true; }
		
	private:
		ftxui::StringRef cmd_;
		std::function<void(std::string)> on_submit_;
		std::function<void()> menu_0_selection;
		std::function<void()> menu_1_selection;
		std::function<void()> menu_2_selection;
		
		int depth = 0;
		
		std::string command = "";
		std::string function = "";
		std::string argument = "";
		int command_index = 0;
		int function_index = 0;
		int argument_index = 0;
		int command_focus = 0;
		int function_focus = 0;
		int argument_focus = 0;
		
		std::vector<std::string> commands;
		std::vector<std::string> arguments;
		std::vector<std::string> ps_functions;
		std::vector<std::string> fs_functions;
		std::vector<std::string> dev_functions;
		std::vector<std::string> net_functions;
		std::vector<std::string> conn_functions;
		std::vector<std::string> msg_functions;
		std::vector<std::vector<std::string>> functions;
		std::vector<std::string>& fn_options = ps_functions;
		
		ftxui::Component main_container;
		ftxui::Component depth_0_renderer;
		ftxui::Component depth_1_renderer;
		ftxui::Component depth_2_renderer;
		ftxui::Component depth_0_container;
		ftxui::Component depth_1_container;
		ftxui::Component depth_2_container;
	};
	
	return ftxui::Make<Impl>(std::move(cmd), std::move(submit));
}

}	// namespace Auto

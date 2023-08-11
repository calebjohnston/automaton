//
//  game.hpp
//
//  Created by Caleb Johnston on 7/28/2023.
//  Copyright © 2023 Caleb Johnston. All rights reserved.
//

#pragma once

#include <chrono>
#include <deque>
#include <functional>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>

#include "ftxui/component/event.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/loop.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/color.hpp"

#include "model.hpp"
#include "render.hpp"
#include "Events.h"

namespace Auto {

ftxui::Component CompoundButton(ftxui::ConstStringRef label,
								std::function<void()> on_click,
								ftxui::Ref<ftxui::ButtonOption> = ftxui::ButtonOption::Simple());

ftxui::Component AnimatedText(ftxui::ConstStringRef label,
							  ftxui::Ref<ftxui::ButtonOption> = ftxui::ButtonOption::Animated());

#pragma game model state

struct Command {
	std::string function;
	Agent* target;
	std::vector<std::string> arguments;
};
// examples:
//	ps
// 	ps load/unload <targ>

class Action {
public:
	virtual ResultSet execute() { return {}; }
};

class ProgramsAction : public Action {
public:
	ProgramsAction(Agent* targ, bool list = true, int index = 0) : _target(targ), _list(list), _idx(index) {}
	virtual ResultSet execute() override {
		if (_list) {
			return list_programs(_target->kernel);
		}
		else { // let's just assume that this one uninstalls...
			return { uninstall_program(_target->kernel, _idx) };
		}
	}
//	ProgramsAction(ProgramsAction& axn) : _target(axn._target), _list(axn._list), _idx(axn._idx) {}
//	ProgramsAction(const ProgramsAction& axn) : _target(axn._target), _list(axn._list), _idx(axn._idx) {}
	
private:
	Agent* _target;
	bool _list;
	int _idx;
};

class ActionProcessor;

struct GameState {
	int current_agent_idx;
	std::vector<Agent> agents;
	
	ActionProcessor* action_proc;
	EventDispatchMap evt_dp;
	std::vector<Action*> actions;
	std::string axn_results;
	std::string animation_test;
	int mode_index;
	
	// test only
	std::deque<Command> ai_cmds;
	bool gameover;
	bool show_cmd_menu;
};

GameState the_game;







class ModelAction : public Action {
public:
	ModelAction(Agent* targ, bool list = true, int index = 0) : _target(targ), _list(list), _idx(index) {}
	virtual ResultSet execute() override {
		the_game.current_agent_idx = 1;
		return { { 0, "Success"} };
		
	}
	//	ProgramsAction(ProgramsAction& axn) : _target(axn._target), _list(axn._list), _idx(axn._idx) {}
	//	ProgramsAction(const ProgramsAction& axn) : _target(axn._target), _list(axn._list), _idx(axn._idx) {}
	
private:
	Agent* _target;
	bool _list;
	int _idx;
};


class NetworkAction : public Action {
public:
	NetworkAction(Agent* targ, int index) : _target(targ), _idx(index) {}
	virtual ResultSet execute() override {
		the_game.mode_index = 1;
		the_game.current_agent_idx = _idx;
		return { { 0, "Success"} };
	}
	
private:
	Agent* _target;
	int _idx;
};

class OffensiveAction : public Action {
public:
	OffensiveAction(Agent* targ, int index) : _target(targ), _idx(index) {}
	virtual ResultSet execute() override {
		the_game.mode_index = 2;
		the_game.current_agent_idx = 1; // CJJ: using 1 instead of _idx to ensure the opponent is the AI;
		return { { 0, "Success"} };
	}
	
private:
	Agent* _target;
	int _idx;
};


class CrapAction : public Action {
public:
	CrapAction(Agent* targ, int index) : _target(targ), _idx(index) {}
	virtual ResultSet execute() override {
		the_game.mode_index = 2;
		the_game.current_agent_idx = 1; // CJJ: using 1 instead of _idx to ensure the opponent is the AI;
		
		if (_target->type == Class::Player) {
			Agent& receiver = the_game.agents[1];
			receiver.kernel.hitpoints -= 1;
			the_game.animation_test = "health:" + std::to_string(receiver.kernel.hitpoints);
		}
		else if (_target->type != Class::Player) {
			Agent& receiver = the_game.agents[0];
			receiver.kernel.hitpoints -= 1;
			the_game.animation_test = "health:" + std::to_string(receiver.kernel.hitpoints);
		}
		
		return { { 0, "Success" } };
	}
	
private:
	Agent* _target;
	int _idx;
};





#pragma utility functions

Command parse(std::string user_input, Agent* user_agent)
{
	std::vector<std::string> args;
	std::istringstream iss(user_input);
	std::string fn_str;
	iss >> fn_str;
	for (std::string arg; iss >> arg; ) args.push_back(arg);
	
	return { fn_str, user_agent, args };
}

#pragma game lifecycle operations

Command decide(Agent& agent)
{
	Command cmd;
	if (agent.type != Class::Player) {
		// cmd = agent must choose what to do and issue a command
		cmd = the_game.ai_cmds.front();
		the_game.ai_cmds.pop_front();
	}
	else if (agent.type == Class::Player) {
		std::string input;
		std::cin >> input;
		// ... get input from user
//		input = "ps";
		cmd = parse(input, &agent);
		// example: cmd = { "ps", &agent, { "unload", "_program_" } };
	}
	
	return cmd;
};

Action* process(Command& cmd)
{
//	using namespace std::placeholders;
//
//	std::deque<std::string> commands;
//	std::deque<std::string> options;
//	std::vector<std::string> command_params = {"", "load", "unload", "install", "uninstall"};
//	std::vector<std::string> option_params = {"-i", "--input", "-o", "--output", "-c", "--config", "-v", "--verbose", "-d", "--default"};
	if ("ps" == cmd.function) {
		if (cmd.arguments.empty()) {
			return new ProgramsAction(cmd.target);
		}
		else if (cmd.arguments.front() == "uninstall") {
			if (cmd.arguments.size() == 2) {
				int index = std::stoi( cmd.arguments[1] ); // big assumption...
				return new ProgramsAction(cmd.target, false, index);
			}
			else {
				// NO-OP
			}
		}
	}
	else if ("fs" == cmd.function) {
		return new ModelAction(cmd.target, false, 1);
	}
	else if ("conn" == cmd.function) {
		int index = std::stoi( cmd.arguments[0] );
		return new NetworkAction(cmd.target, index);
	}
	else if ("attack" == cmd.function) {
		int index = std::stoi( cmd.arguments[0] );
		return new OffensiveAction(cmd.target, index);
	}
	else if ("ping" == cmd.function) {
		int index = std::stoi( cmd.arguments[0] );
		return new CrapAction(cmd.target, index);
	}
	
	// perform GameState model lookups on command function and target
	// parse arguments from string inputs
	
	return new Action();
}

ResultSet execute(Action& action)
{
	return action.execute();
};

#pragma the game itself

typedef std::shared_ptr<class ActionEvent> ActionEventRef;

class ActionEvent : public Event {
public:
	static const EventId ACTION;
	
public:
	static ActionEventRef create( const EventId id, Action* action ) {
		return ActionEventRef( new ActionEvent(id, action) );
	}
	
	virtual ~ActionEvent() {};
	
	Action* getAction() const { return _action; }
	
protected:
	explicit ActionEvent( const EventId id, Action* action )
		: Event(id, 0), _action(action) {}
	
private:
	Action* _action;
};

class ActionProcessor {
public:
	void handler(const EventRef event) {
		auto action_evt = std::dynamic_pointer_cast<ActionEvent>(event);
		if (action_evt) {
			_action = action_evt->getAction();
			the_game.actions.push_back(_action);
		}
	}
	
private:
	Action* _action;
};

std::hash<std::string> str_hash;
const EventId ActionEvent::ACTION = str_hash( "ActionProcessorEvent" );

void load_gamestate()
{
	the_game.current_agent_idx = 0;
	the_game.mode_index = 0;
	
	// the human game player...
	Auto::Device disk = { "Hitachi 500", Component::Disk, 100, 16 };
	Auto::Device mem = { "Crucial 64GB", Component::Memory, 64, 8 };
	Auto::Device cpu = { "Ono Sendai 2155", Component::Processor, 32, 128 };
	Auto::Device net = { "DEC OC 768", Component::Network, 4, 12 };
	Software ping = { "ping", "ICMP network control", 10, 1, 1, Packet::Ping, Binary::Program, Encryption::Kerberos };
	Software cfg = { "cfg", "System control", 5, 1, 1, Packet::Symplex, Binary::Program, Encryption::None };
	File f1 = { "Dall-E2", "Model weights for image GAN", "", 4, 100 };
	File f2 = { "Emails", "All my personal emails", "", 0, 4 };
	Computer comp = { "567g8k", "Apple", disk, mem, cpu, net, {}, { "Gigabyte", Component::Power, 100, 100 } };
	Kernel system = { "MacOS", { ping, cfg }, {}, { f1, f2 }, {}, 1, comp, 5 };
	Agent player = { "caleb", "author", Auto::Status::Active, Auto::Class::Player, 1, system };
	the_game.agents.push_back(player);
	
	// the one AI opponent...
	disk = { "Intel 4080", Component::Disk, 100, 16 };
	mem = { "Kingston 64i8ab", Component::Memory, 64, 8 };
	cpu = { "Intel i7", Component::Processor, 20, 128 };
	net = { "Cisco RT44", Component::Network, 4, 12 };
	Software inf = { "inf", "Inference ctx control", 10, 1, 1, Packet::Inference, Binary::Program, Encryption::Kerberos };
	Software simplex = { "xym", "Symplex backplane", 10, 1, 1, Packet::Symplex, Binary::Daemon, Encryption::None };
	Software inet = { "inet", "syscrtl inode", 5, 1, 1, Packet::Reflection, Binary::Daemon, Encryption::None };
	f1 = { "Dall-E2", "Model weights for image GAN", "", 4, 100 };
	f2 = { "Crypto DB", "Database containing an index for crypto algos", "", 1, 10 };
	Computer comp2 = { "nkpasd8", "IBM", disk, mem, cpu, net, {}, { "Gigabyte", Component::Power, 100, 100 } };
	Kernel system2 = { "OS/2", { inf }, { simplex, inet }, { f1, f2 }, {}, 1, comp2, 10 };
	Agent ai_player = { "wintermute", "victor", Auto::Status::Active, Auto::Class::Automaton, 1, system2 };
	the_game.agents.push_back(ai_player);
	
	// connect game threads with observer pattern
	using namespace std::placeholders;
	the_game.action_proc = new ActionProcessor();
	the_game.evt_dp.appendListener(ActionEvent::ACTION, std::bind(&ActionProcessor::handler, the_game.action_proc, _1));
	
	Agent* agent_ptr = &the_game.agents.back();
	the_game.ai_cmds.push_back(parse("ps", agent_ptr));
	the_game.ai_cmds.push_back(parse("fs", agent_ptr));
	the_game.ai_cmds.push_back(parse("attack 0", agent_ptr));
	the_game.ai_cmds.push_back(parse("ping 0", agent_ptr));
	the_game.ai_cmds.push_back(parse("ping 0", agent_ptr));
	the_game.ai_cmds.push_back(parse("ping 0", agent_ptr));
	the_game.ai_cmds.push_back(parse("ping 0", agent_ptr));
	the_game.ai_cmds.push_back(parse("ping 0", agent_ptr));
	
	the_game.gameover = false;
	the_game.show_cmd_menu = false;
};

bool is_game_over() {
	// is player dead?
	return the_game.agents[0].kernel.hitpoints <= 0;
}


ftxui::Component ModalComponent(std::function<void()> do_nothing, std::function<void()> quit)
{
	using namespace ftxui;
	
	auto style = ButtonOption::Animated();
	auto component = Container::Vertical({
		Button("Retry", do_nothing, style),
		Button("Quit", quit, style),
	}) | Renderer([&](Element inner) {
		return vbox({
			text("Game Over"),
			separator(),
			inner,
		})
		| size(WIDTH, GREATER_THAN, 30)
		| border;
	});
	
	return component;
}

ftxui::Component MenuModalComponent(std::function<void()> left, std::function<void()> right)
{
	using namespace ftxui;
	
	auto style = ButtonOption::Animated();
	auto component = Container::Vertical({
		Auto::CompoundButton("Retry", left, style),
		Button("Quit", right, style),
	}) | Renderer([&](Element inner) {
		return vbox({
			text("Command Menu"),
			separator(),
			inner,
		})
//		| size(WIDTH, GREATER_THAN, 30)
		| border;
	});
	
	return component;
}

ftxui::Element agent_view(Agent& agent)
{
	using namespace ftxui;
	
	return vbox({
		window(text(" Agent "), vbox({
			text(agent.name),
			text(agent.description),
			text(to_str(agent.type)),
			text(to_str(agent.status)),
			text("version:" + std::to_string(agent.version))
		})),
		window(text(" System "), vbox({
			text("name:" + agent.kernel.hostname),
			text("health:" + std::to_string(agent.kernel.hitpoints)),
			text("programs: " + std::to_string(agent.kernel.programs.size())),
			text("daemons: " + std::to_string(agent.kernel.daemons.size())),
			text("files: " + std::to_string(agent.kernel.files.size())),
			text("peers: " + std::to_string(agent.kernel.connections.size())),
		})),
		window(text(" Computer "), vbox({
			text(agent.kernel.computer.processor.name),
			text(agent.kernel.computer.memory.name),
			text(agent.kernel.computer.disk.name),
			text(agent.kernel.computer.uplink.name),
			text(agent.kernel.computer.serial),
		}))
	});
}

/**
 This version splits off the GUI renderer onto its own thread
 The renderer should then just handle re-rendering model changes in an MVC pattern where
	the renderer is the view, the action processor is the controller, and the game_state is the model
 */
void gameplay_loop()
{
	using namespace ftxui;
	
	bool game_over = false;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	
	std::thread gui([&] {
		while (!game_over) {
			for (Agent& agent : the_game.agents) {
				if (agent.type != Class::Player) {
					Command command = decide(agent);
					Action* action = process(command);
					ResultSet results = execute(*action);
				}
				else if (agent.type == Class::Player) {
					while (the_game.actions.empty()) {
						using namespace std::chrono_literals;
						std::this_thread::sleep_for(0.1s);
//						the_game.msgr->update();
					}
					for (Action* axn_ptr : the_game.actions) {
						ResultSet results = execute(*axn_ptr);
						the_game.axn_results = results[0].message;
						
						screen.Post(ftxui::Event::Custom);	// <!-- triggers a re-render
						
						// log results to file?
					}
					the_game.actions.clear();
				}
			}
			game_over = is_game_over();
			the_game.gameover = game_over;
		}
	});
	
	bool detail = false;
	auto action = [&] { the_game.show_cmd_menu = !the_game.show_cmd_menu; };
	auto btn = Button("Render Toggle", action, ButtonOption::Ascii());
	
	auto modal_component = ModalComponent([]{}, screen.ExitLoopClosure());
	auto menu_modal_component = MenuModalComponent([]{}, []{});
	
	
//	auto animation_text = "health:" + std::to_string(the_game.agents[0].kernel.hitpoints);
	the_game.animation_test = "health:" + std::to_string(the_game.agents[0].kernel.hitpoints);
	auto animated_field = AnimatedText(&the_game.animation_test);
	
	auto dev = Renderer([&] {
		Agent& agent = the_game.agents[0];
		Elements programs, files;
		std::for_each(agent.kernel.programs.begin(), agent.kernel.programs.end(), [&](Software& sw) {
			detail ? programs.push_back(text(sw.name + "   " + sw.description)) : programs.push_back(text(sw.name));
		});
		std::for_each(agent.kernel.files.begin(), agent.kernel.files.end(), [&](File& file) {
			detail ? files.push_back(text(file.name + "   " + file.description)) : files.push_back(text(file.name));
		});
		
		return hbox({
			vbox(programs) | flex,
			vbox(files) | flex,
			agent_view(agent)
		});
	});
	
	
	auto net = Renderer([&] {
		Agent& player_agent = the_game.agents[0];
		Agent& remote_agent = the_game.agents[the_game.current_agent_idx];
		Elements local, remote;
		std::for_each(player_agent.kernel.files.begin(), player_agent.kernel.files.end(), [&](File& file) {
			detail ? local.push_back(text(file.name + "   " + file.description)) : local.push_back(text(file.name));
		});
		std::for_each(remote_agent.kernel.files.begin(), remote_agent.kernel.files.end(), [&](File& file) {
			detail ? remote.push_back(text(file.name + "   " + file.description)) : remote.push_back(text(file.name));
		});
		
		Agent& agent = remote_agent;
		return hbox({
			vbox(local) | flex,
			vbox(remote) | flex,
			agent_view(agent)
		});
	});
	
	auto hack = Renderer([&] {
		Agent& local = the_game.agents[0];
		Agent& remote = the_game.agents[the_game.current_agent_idx];
		
		// CJJ: this won't work because SetLabel is a method of an internal Impl class, not an AnimatedText class :(
//		auto anim_field_ptr = std::dynamic_pointer_cast<AnimatedText>(animated_field);
//		if (anim_field_ptr) anim_field_ptr->SetLabel("health:" + std::to_string(local.kernel.hitpoints));
		
		std::string para_str =
			"Lorem Ipsum is simply dummy text of the printing and typesetting "
			"industry. Lorem Ipsum has been the industry's standard dummy text "
			"ever since the 1500s, when an unknown printer took a galley of type "
			"and scrambled it to make a type specimen book.";
		return hbox({
			agent_view(local),
			vbox({
				window(text("Align left:"), paragraphAlignLeft(para_str))
			}) | flex,
			agent_view(remote),
			animated_field->Render()
		});
	});
	
	std::string input_str;
	
	auto input_option = ftxui::InputOption();
	input_option.on_enter = [&] {
		Command cmd = parse(input_str, &the_game.agents[0]); // <-- GAaah
		Action* action = process(cmd);
		the_game.evt_dp.dispatch(ActionEvent::ACTION, ActionEvent::create(ActionEvent::ACTION, action));
		input_str = "";
	};
	auto input_command = ftxui::Input(&input_str, "_", input_option);
	
	
	std::vector<std::string> tab_entries = { "Development", "Networking", "Infiltration" };
	auto tab_selection = Menu(&tab_entries, &the_game.mode_index, MenuOption::HorizontalAnimated());
	auto tab_content = Container::Tab({ dev, net, hack }, &the_game.mode_index);
	
	auto main_container = Container::Vertical({
		tab_selection,
		tab_content,
		Container::Vertical({ input_command, btn, animated_field })
	});
	
	auto renderer = Renderer(main_container, [&] {
		return vbox({
			text("Automaton") | bold | hcenter,
			tab_selection->Render(),
			tab_content->Render() | flex,
			separatorHSelector(0, 0, Color::Palette256::Red1, Color::Palette256::SeaGreen1),
			hbox({
				input_command->Render() | flex,
				btn->Render()
			})
		});
	});
	
	screen.Loop(renderer | Modal(modal_component, &the_game.gameover) | Modal(menu_modal_component, &the_game.show_cmd_menu));
	
	gui.join();
};


/**
 NEXT STEPS:
	* create a custom Button component that has multiple children - DONE
	* get some basic output animations working - DONE
	- create a menu system for creating commands from selections (see modal_dialog_custom & dbox)
	- update the battle system to incorporate installed daemons
	- update the battle system so that only installed programs can be used
 */

}

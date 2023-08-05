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
	int mode_index;
	
	// test only
	std::deque<Command> ai_cmds;
	bool gameover;
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
		}
		else if (_target->type != Class::Player) {
			Agent& receiver = the_game.agents[0];
			receiver.kernel.hitpoints -= 1;
		}
		
		return { { 0, "Success"} };
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
};

bool is_game_over() {
	// is player dead?
	return the_game.agents[0].kernel.hitpoints <= 0;
}


/**
 This implementation works and presents a good single-threaded model for our expected gameplay interactive
 However, it cannot render UI animations or handle any state changes from a 3rd party GUI (e.g. Godot, FXTUI)
 */
void gameplay_loop()
{
//	std::string cmd_str;
//	auto gui = ftxui::Container::Vertical({ftxui::Input(&cmd_str, "_")});
//	auto screen = ftxui::ScreenInteractive::TerminalOutput();
//	ftxui::Loop loop(&screen, gui);
	
	bool game_over = false;
	while (!game_over) {
		for (Agent& agent : the_game.agents) {
			Command command = decide(agent);
			Action* action = process(command);
			ResultSet results = execute(*action);
			if (agent.type == Class::Player)
				render_stdio(results); // loop.RunOnce(); // <-- I think this needs to be split in half
		}
		game_over = is_game_over();
		// game_over = check for game termination condition...
	}
};

/**
 This implementation doesn't work as desired.
 It can capture user inputs and render outputs but will not work with out-of-band events (e.g. opponent actions)
 */
void gameplay_loop_0()
{
	std::string cmd_str;
	std::string input_str;
	std::string output_str;
	
	ResultSet results;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	auto input_option = ftxui::InputOption();
	input_option.on_enter = [&] {
		cmd_str = input_str;
		Command cmd = parse(cmd_str, &the_game.agents[0]); // <-- GAaah
		Action* action = process(cmd);
		results = execute(*action);
		input_str = "";
		output_str = render_str(results);
	};
	auto input_command = ftxui::Input(&input_str, "_", input_option);
	auto component = ftxui::Container::Vertical({ input_command });
	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({
			ftxui::text(output_str),
			ftxui::separator(),
			input_command->Render(),
		}) | ftxui::border;
	});
	
	ftxui::Loop loop(&screen, renderer);
	
	// The game can't be run 
	bool game_over = false;
	while (!game_over) {
		for (Agent& agent : the_game.agents) {
			if (agent.type == Class::Player) {
				loop.RunOnce();
			}
//			else {
//				output_str = agent.name;
//				loop.RunOnce();
//			}
		}
	}
	
//	screen.Loop(renderer);
}

/**
 This implementation doesn't work because its not interactive
 It renders outputs but it will not capture inputs
 */
void gameplay_loop_1()
{
	std::string reset_position;
	
	std::string cmd_str;
	std::string input_str;
	std::string output_str;
	
	// fetch the player agent ...
	Agent* player_agent;
	for (Agent& agent : the_game.agents) {
		if (agent.type == Class::Player) {
			player_agent = &agent;
		}
	}
	
	// configure GUI ...
	ResultSet results;
	auto input_option = ftxui::InputOption();
	input_option.on_enter = [&] {
		cmd_str = input_str;
		Command cmd = parse(cmd_str, player_agent);
		Action* action = process(cmd);
		results = execute(*action);
		input_str = "";
		output_str = render_str(results);
	};
	auto input_command = ftxui::Input(&input_str, "_", input_option);
	auto component = ftxui::Container::Vertical({ input_command });
	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({
			ftxui::text(output_str),
			ftxui::separator(),
			input_command->Render(),
		}) | ftxui::border;
	});
	
	// game loop ...
	bool game_over = false;
	while (!game_over) {
		for (Agent& agent : the_game.agents) {
			if (agent.type != Class::Player) {
				Command command = decide(agent);
				Action* action = process(command);
				ResultSet results = execute(*action);
				continue;
			}
		
			auto document = renderer->Render();
			auto screen = ftxui::ScreenInteractive::FixedSize(80, 25);
			ftxui::Render(screen, document);
			std::cout << reset_position;
			screen.Print();
			reset_position = screen.ResetPosition();
		}
		// game_over = check for game termination condition...
	}
}

std::string _output_str;

ftxui::Element gameloop_fn(ftxui::Element el)
{
	for (Agent& agent : the_game.agents) {
		if (agent.type == Class::Player) continue;
		Command command = decide(agent);
		Action* action = process(command);
		ResultSet results = execute(*action);
		_output_str = agent.name;
	}
	return el;
}

/**
 This implementation doesn't work as desired
 - The FXTUI loop still runs as long as their are ANY inputs or model changes
 - So the player will only supply a command as many times as they hit the <enter> key, but the opponents will submit commands upon every single mouse move, key press, or window resize event (etc)
 */
void gameplay_loop_2()
{
	std::string cmd_str;
	std::string input_str;
	
	ResultSet results;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	auto input_option = ftxui::InputOption();
	input_option.on_enter = [&] {
		cmd_str = input_str;
		Command cmd = parse(cmd_str, &the_game.agents[0]); // <-- GAaah
		Action* action = process(cmd);
		results = execute(*action);
		input_str = "";
		_output_str = render_str(results);
	};
	auto input_command = ftxui::Input(&input_str, "_", input_option);
	auto component = ftxui::Container::Vertical({ input_command });
	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({
			ftxui::text(_output_str),
			ftxui::separator(),
			input_command->Render(),
		}) | ftxui::border | gameloop_fn;
	});
	
	screen.Loop(renderer);
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


/**
 This version splits off the GUI renderer onto its own thread
 The renderer should then just handle re-rendering model changes in an MVC pattern where
	the renderer is the view, the action processor is the controller, and the game_state is the model
 */
void gameplay_loop_3()
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
//			if (game_over) screen.ExitLoopClosure()();	// Works! but not friendly
		}
	});
	
	bool detail = false;
	auto action = [&] { detail = !detail; };
	auto btn = Button("Render Toggle", action, ButtonOption::Ascii());
	
	auto modal_component = ModalComponent([]{}, screen.ExitLoopClosure());
	Modal(modal_component, &the_game.gameover);
	
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
			vbox({
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
			})
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
			vbox({
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
			})
		});
	});
	
	auto hack = Renderer([&] {
		Agent& local = the_game.agents[0];
		Agent& remote = the_game.agents[the_game.current_agent_idx];
		
		std::string para_str =
			"Lorem Ipsum is simply dummy text of the printing and typesetting "
			"industry. Lorem Ipsum has been the industry's standard dummy text "
			"ever since the 1500s, when an unknown printer took a galley of type "
			"and scrambled it to make a type specimen book.";
		return hbox({
			vbox({
				window(text(" Agent "), vbox({
					text(local.name),
					text(local.description),
					text(to_str(local.type)),
					text(to_str(local.status)),
					text("version:" + std::to_string(local.version))
				})),
				window(text(" System "), vbox({
					text("name:" + local.kernel.hostname),
					text("health:" + std::to_string(local.kernel.hitpoints)),
					text("programs: " + std::to_string(local.kernel.programs.size())),
					text("daemons: " + std::to_string(local.kernel.daemons.size())),
					text("files: " + std::to_string(local.kernel.files.size())),
					text("peers: " + std::to_string(local.kernel.connections.size())),
				})),
				window(text(" Computer "), vbox({
					text(local.kernel.computer.processor.name),
					text(local.kernel.computer.memory.name),
					text(local.kernel.computer.disk.name),
					text(local.kernel.computer.uplink.name),
					text(local.kernel.computer.serial),
				}))
			}),
			vbox({
				window(text("Align left:"), paragraphAlignLeft(para_str))
			}) | flex,
			vbox({
				window(text(" Agent "), vbox({
					text(remote.name),
					text(remote.description),
					text(to_str(remote.type)),
					text(to_str(remote.status)),
					text("version:" + std::to_string(remote.version))
				})),
				window(text(" System "), vbox({
					text("name:" + remote.kernel.hostname),
					text("health:" + std::to_string(remote.kernel.hitpoints)),
					text("programs: " + std::to_string(remote.kernel.programs.size())),
					text("daemons: " + std::to_string(remote.kernel.daemons.size())),
					text("files: " + std::to_string(remote.kernel.files.size())),
					text("peers: " + std::to_string(remote.kernel.connections.size())),
				})),
				window(text(" Computer "), vbox({
					text(remote.kernel.computer.processor.name),
					text(remote.kernel.computer.memory.name),
					text(remote.kernel.computer.disk.name),
					text(remote.kernel.computer.uplink.name),
					text(remote.kernel.computer.serial),
				}))
			})
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
		Container::Vertical({ input_command, btn })
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
	
	screen.Loop(renderer | Modal(modal_component, &the_game.gameover));
	
	gui.join();
};



}

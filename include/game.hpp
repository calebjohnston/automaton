//
//  game.hpp
//
//  Created by Caleb Johnston on 7/28/2023.
//  Copyright © 2023 Caleb Johnston. All rights reserved.
//

#pragma once

#include <chrono>
#include <functional>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "ftxui/component/event.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/loop.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/color.hpp"

#include "model.hpp"
#include "render.hpp"
#include "EventQueue.h"
#include "EventDispatcher.h"

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
	std::vector<Agent> agents;
	
//	Messenger* msgr;
	ActionProcessor* action_proc;
	EventDispatcher evt_dp;
	std::vector<Action*> actions;
	std::string axn_results;
};

#pragma utility functions

Command parse(std::string& user_input, Agent* user_agent)
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
		// NO-OP
	}
	else if ("cfg" == cmd.function) {
		// NO-OP
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

GameState the_game;

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
	// the human game player...
	Auto::Device dev;
	Software ping = { "ping", "ICMP network control", 10, 1, 1, Packet::Ping, Binary::Program, Encryption::Kerberos };
	Software cfg = { "cfg", "System control", 5, 1, 1, Packet::Symplex, Binary::Program, Encryption::None };
	Computer comp = { "serial", "manufacturer", dev, dev, { "Intel i7", Component::Processor, 20, 1 }, dev, dev, { "Gigabyte", Component::Power, 100, 100 } };
	Kernel system = { "hostname", { ping, cfg }, {}, {}, {}, 1, comp };
	Agent player = { "name", "description", Auto::Status::Active, Auto::Class::Player, 1, system };
	the_game.agents.push_back(player);
	
	// the one AI opponent...
	Software inf = { "inf", "Inference ctx control", 10, 1, 1, Packet::Inference, Binary::Program, Encryption::Kerberos };
	Computer comp2 = { "serial", "manufacturer", dev, dev, { "Intel i7", Component::Processor, 20, 1 }, dev, dev, { "Gigabyte", Component::Power, 100, 100 } };
	Kernel system2 = { "hostname", { inf }, {}, {}, {}, 1, comp2 };
	Agent ai_player = { "wintermute", "descr", Auto::Status::Active, Auto::Class::Automaton, 1, system2 };
	the_game.agents.push_back(ai_player);
	
	// connect game threads with observer pattern
	using namespace std::placeholders;
	the_game.action_proc = new ActionProcessor();
//	the_game.msgr = Messenger::instance();
//	the_game.msgr->addListener(std::bind(&ActionProcessor::handler, the_game.action_proc, _1), ActionEvent::ACTION);
	the_game.evt_dp.addListener(std::bind(&ActionProcessor::handler, the_game.action_proc, _1), ActionEvent::ACTION);
};


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
		game_over = true;
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
			// game_over = check for game termination condition...
		}
	});
	
	/*
	input_command->Active();
	auto component = ftxui::Container::Vertical({ input_command });
	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({
			ftxui::text(the_game.axn_results),
			ftxui::separator(),
			input_command->Render(),
		}) | ftxui::border;
	});
	*/
	
	
	auto dev = Renderer([&] {
		return hbox({
			vbox({
				text("installed program 1"),
				text("installed program 2"),
				text("installed program 3"),
				text("installed program 4")
			}),
			filler(),
			vbox({
				text("available program 1"),
				text("available program 2"),
				text("available program 3"),
				text("available program 4"),
				text("available program 5"),
				text("available program 6")
			}),
			filler(),
			vbox({
				window(text("System Stats"), text("whatever")),
				window(text("Capacity"), text("whatever")),
				window(text("Some other stuff"), text("whatever"))
			})
		});
	});
	
	auto net = Renderer([&] {
		return hbox({
			vbox({
				text("installed program 1"),
				text("installed program 2"),
				text("installed program 3"),
				text("installed program 4"),
				text("installed program 5"),
				text("installed program 6")
			}),
			filler(),
			vbox({
				text("remote program X"),
				text("remote program Y"),
				text("remote program Z")
			}),
			filler(),
			vbox({
				window(text("Remote Sys Stats"), text("whatever")),
				window(text("Capacity"), text("and ")),
				window(text("Some other stuff"), text("stuff"))
			})
		});
	});
	
	auto hack = Renderer([&] {
		std::string para_str =
			"Lorem Ipsum is simply dummy text of the printing and typesetting "
			"industry. Lorem Ipsum has been the industry's standard dummy text "
			"ever since the 1500s, when an unknown printer took a galley of type "
			"and scrambled it to make a type specimen book.";
		return hbox({
			vbox({
				window(text("My Sys Stats"), text("whatever")),
				window(text("Capacity"), text("and ")),
				window(text("Some other stuff"), text("stuff"))
			}),
			filler(),
			vbox({
				window(text("Align left:"), paragraphAlignLeft(para_str))
			}),
			filler(),
			vbox({
				window(text("Remote Sys Stats"), text("whatever")),
				window(text("Capacity"), text("and ")),
				window(text("Some other stuff"), text("stuff"))
			})
		});
	});
	
	std::string input_str;
	
	auto input_option = ftxui::InputOption();
	input_option.on_enter = [&] {
		Command cmd = parse(input_str, &the_game.agents[0]); // <-- GAaah
		Action* action = process(cmd);
		the_game.evt_dp.dispatch(ActionEvent::create(ActionEvent::ACTION, action));
		input_str = "";
	};
	auto input_command = ftxui::Input(&input_str, "_", input_option);
	
	
	int tab_index = 0;
	std::vector<std::string> tab_entries = { "Development", "Networking", "Infiltration" };
	auto tab_selection = Menu(&tab_entries, &tab_index, MenuOption::HorizontalAnimated());
	auto tab_content = Container::Tab({ dev, net, hack }, &tab_index);
	
	auto main_container = Container::Vertical({
		tab_selection,
		tab_content,
		input_command
	});
	
	auto renderer = Renderer(main_container, [&] {
		return vbox({
			text("Automaton") | bold | hcenter,
			tab_selection->Render(),
			tab_content->Render() | flex,
			separatorHSelector(0, 0, Color::Palette256::Red1, Color::Palette256::SeaGreen1),
			input_command->Render()
		});
	});
	
	screen.Loop(renderer);
	
	gui.join();
};



}

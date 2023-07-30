//
//  game.hpp
//
//  Created by Caleb Johnston on 7/28/2023.
//  Copyright © 2023 Caleb Johnston. All rights reserved.
//

#pragma once

#include <functional>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/loop.hpp"

#include "model.hpp"
#include "render.hpp"

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
		else {
			return { 0, info(_target->kernel.programs[_idx]) };
		}
	}
//	ProgramsAction(ProgramsAction& axn) : _target(axn._target), _list(axn._list), _idx(axn._idx) {}
//	ProgramsAction(const ProgramsAction& axn) : _target(axn._target), _list(axn._list), _idx(axn._idx) {}
	
private:
	Agent* _target;
	bool _list;
	int _idx;
};

struct GameState {
	std::vector<Agent> agents;
//	std::vector<Command> commands;
//	std::vector<Action> actions;
//	std::vector<Result> results;
	
//	ftxui::ScreenInteractive screen;
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
		else {
			int index = std::stoi( cmd.arguments[1] ); // big assumption...
			return new ProgramsAction(cmd.target, false, index);
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

void load_gamestate()
{
	Auto::Device dev;
	Software ping = { "ping", "ICMP network control", 10, 1, 1, Packet::Ping, Binary::Program, Encryption::Kerberos };
	Software cfg = { "cfg", "System control", 5, 1, 1, Packet::Symplex, Binary::Program, Encryption::None };
	Computer comp = { "serial", "manufacturer", dev, dev, { "Intel i7", Component::Processor, 20, 1 }, dev, dev, { "Gigabyte", Component::Power, 100, 100 } };
	Kernel system = { "hostname", { ping, cfg }, {}, {}, {}, 1, comp };
	Agent player = { "name", "description", Auto::Status::Active, Auto::Class::Player, 1, system };
	the_game.agents.push_back(player);
};

void gameplay_loop()
{
	std::string cmd_str;
	auto gui = ftxui::Container::Vertical({ftxui::Input(&cmd_str, "_")});
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	ftxui::Loop loop(&screen, gui);
	
	bool game_over = false;
	while (!game_over) {
		for (Agent& agent : the_game.agents) {
			Command command = decide(agent);
			Action* action = process(command);
			ResultSet results = execute(*action);
			if (agent.type == Class::Player)
				render(results); // loop.RunOnce(); // <-- I think this needs to be split in half
		}
		game_over = true;
		// game_over = check for game termination condition...
	}
};

}

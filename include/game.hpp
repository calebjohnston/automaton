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
#include <vector>

#include "model.hpp"

namespace Auto {

struct Command {
	std::string function;
	std::string target;
	std::vector<std::string> arguments;
};

struct Action {
	std::function<Result()> function;
};

struct GameState {
	std::vector<Agent> agents;
	std::vector<Command> commands;
	std::vector<Action> actions;
	std::vector<Result> results;
	// GUI placeholder...
};

Command make_decision(Agent& agent)
{
	Command cmd;
	if (agent.type != Class::Player) {
		// cmd = agent must choose what to do and issue a command
	}
	else if (agent.type == Class::Player) {
		// cmd = solicit input from the player and conver it to a command
	}
	
	return cmd;
};

Action process_command(Command& cmd)
{
	Action axn;
	
	// perform GameState model lookups on command function and target
	// parse arguments from string inputs
	
	return axn;
}

Result execute_action(Action& action)
{
	return action.function();
};

GameState the_game;

void load_gamestate()
{
	
};

void gameplay_loop()
{
	for (Agent& agent : the_game.agents) {
		Command command = make_decision(agent);
		Action action = process_command(command);
		Result result = execute_action(action);
		if (agent.type == Class::Player) {
			// Render results using GUI
		}
		// Test for game termination condition...
	}
};

}

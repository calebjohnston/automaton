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

#pragma game model state

struct Command {
	std::string function;
	Agent* target;
	std::vector<std::string> arguments;
};
// examples:
//	ps
// 	ps load/unload <targ>

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

#pragma utility functions

Command parse(std::string& user_input, Agent* user_agent)
{
	// parse first token
	// parse arguments
	return { "", user_agent, {} };
}

#pragma game lifecycle operations

Command decide(Agent& agent)
{
	Command cmd;
	if (agent.type != Class::Player) {
		// cmd = agent must choose what to do and issue a command
	}
	else if (agent.type == Class::Player) {
		// cmd = solicit input from the player and conver it to a command
		cmd = { "ps", &agent, {} };
//		cmd = { "ps", &agent, { "unload", "_program_" } };
	}
	
	return cmd;
};

Action process(Command& cmd)
{
	Action axn;
	
	// perform GameState model lookups on command function and target
	// parse arguments from string inputs
	
	return axn;
}

Result execute(Action& action)
{
	return action.function();
};

void render(Result& result)
{
	// TBD
}

GameState the_game;

void load_gamestate()
{
	
};

void gameplay_loop()
{
	bool game_over = false;
	while (!game_over) {
		for (Agent& agent : the_game.agents) {
			Command command = decide(agent);
			Action action = process(command);
			Result result = execute(action);
			if (agent.type == Class::Player)
				render(result);
		}
		// game_over = check for game termination condition...
	}
};

}

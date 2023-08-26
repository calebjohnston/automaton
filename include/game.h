//
//  game.h
//
//  Created by Caleb Johnston on 8/19/2023.
//  Copyright © 2023 Caleb Johnston. All rights reserved.
//

#pragma once

#include <deque>
#include <functional>
#include <numeric>
#include <string>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/dom/elements.hpp"

#include "model.h"
#include "tree.hpp"

namespace Auto {

ftxui::Component CompoundButton(ftxui::ConstStringRef label,
								std::function<void()> on_click,
								ftxui::Ref<ftxui::ButtonOption> = ftxui::ButtonOption::Simple());

ftxui::Component AnimatedText(ftxui::ConstStringRef label,
							  ftxui::Ref<ftxui::ButtonOption> = ftxui::ButtonOption::Animated());

ftxui::Component AnimatedBackground(ftxui::Ref<ftxui::ButtonOption> = ftxui::ButtonOption::Animated());

ftxui::Component CommandPalette(ftxui::StringRef cmd, std::function<void(std::string)> submit, CommandNode node);

ftxui::ComponentDecorator Layer(ftxui::Component layer, const bool* show_layer);

#pragma game model state

class GameState {
public:
	int current_agent_idx;
	std::vector<Agent> agents;
	CommandNode cmd_gui;
	Node cmd_api;
	bool cmd_api_bit;
	
	std::deque<std::string> cmd_history;
	std::string axn_results;
	std::string animation_test;
	int mode_index;
	
	// test only
	std::deque<Command> ai_cmds;
	bool gameover;
	bool show_cmd_menu;
};

static GameState the_game;

#pragma utility functions

void append_to_history(const Command& cmd);
void append_to_history(const std::string& str);
int parse(std::string& num_str);

Command parse(std::string user_input, Agent* user_agent);

#pragma game lifecycle operations

Command decide(Agent& agent);
ResultSet process(Command& cmd);

#pragma the game itself

void build_cmd_api();
void build_cmd_gui();
void load_gamestate();
bool is_game_over();

ftxui::Component ModalComponent(std::function<void()> do_nothing, std::function<void()> quit);
ftxui::Component MenuModalComponent(std::function<void(std::string cmd)> fn);
ftxui::Element agent_view(Agent& agent);

void gameplay_loop();

}	// namespace Auto

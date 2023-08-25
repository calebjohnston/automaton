//
//  game.cpp
//
//  Created by Caleb Johnston on 8/19/2023.
//  Copyright © 2023 Caleb Johnston. All rights reserved.
//

#include <algorithm>
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

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/mouse.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/loop.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/color.hpp"

#include "render.hpp"
#include "game.h"
#include "tree.hpp"

namespace Auto {

#pragma game model state
/*
class InvalidAction : public Action {
public:
	InvalidAction(Command cmd, std::string msg) : _cmd(cmd), _message(msg) {}
	virtual ResultSet execute() { return {}; }
	std::string msg() const { return _message; }
	
private:
	Command _cmd;
	std::string _message;
};

class ProgramsAction : public Action {
public:
	ProgramsAction(Agent* targ, bool list = true, const std::string& prog_name = "", int index = -1) : _target(targ), _list(list), _program_name(prog_name), _idx(index) {}
	virtual ResultSet execute() override {
		if (_list) {
			return list_programs(_target->kernel);
		}
		else if (_idx < 0 && !_program_name.empty()) {
			return { uninstall_program(_target->kernel, _program_name) };
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
	std::string _program_name;
};


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
*/


#pragma utility functions

void append_to_history(const Command& cmd)
{
	std::string cmd_str = cmd.target->name + "$ " + cmd.function;
	cmd_str = std::accumulate(cmd.arguments.begin(), cmd.arguments.end(), cmd_str, [](std::string acc_out, const std::string& arg){ return acc_out += " " + arg; });
	the_game.cmd_history.push_back(cmd_str);
	while (the_game.cmd_history.size() > 5)
		the_game.cmd_history.pop_front();
}

void append_to_history(const std::string& str)
{
	the_game.cmd_history.push_back(str);
	while (the_game.cmd_history.size() > 5)
		the_game.cmd_history.pop_front();
}

int parse(std::string& num_str)
{
	try {
		return std::stoi(num_str);
	}
	catch (std::invalid_argument const& ex) {
		return -1;
	}
	catch (...) {
		return -1;
	}
}

Command parse(std::string user_input, Agent* user_agent)
{
	std::vector<std::string> args;
	std::istringstream iss(user_input);
	std::string fn_str;
	iss >> fn_str;
	for (std::string arg; iss >> arg; ) args.push_back(arg);
	
	return { fn_str, user_agent, args };
}

Result validate(Command& cmd)
{
	return the_game.cmd_api->validate(cmd);
}

Result autocomplete(Command& cmd)
{
	return the_game.cmd_gui->autocomplete(cmd);
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

/*
Action* process(Command& cmd)
{
	// TODO: right now all arguments must be indices. But they can also be names
	
	if ("exit" == cmd.function) {
		std::exit(EXIT_SUCCESS);
	}
	else if ("ps" == cmd.function) {
		if (cmd.arguments.empty()) {
			return new ProgramsAction(cmd.target);
		}
		else if (cmd.arguments.front() == "uninstall") {
			if (cmd.arguments.size() == 2) {
				int index = parse( cmd.arguments[1] );
				if (index < 0)
					return new ProgramsAction(cmd.target, false, cmd.arguments[1]);
				else
					return new ProgramsAction(cmd.target, false, "", index);
			}
			else {
				return new InvalidAction(cmd, "must supply target program or daemon to uninstall");
			}
		}
		else {
			return new InvalidAction(cmd, "unrecognized argument: `" + cmd.arguments.front() + "`");
		}
	}
	else if ("fs" == cmd.function) {
		return new ModelAction(cmd.target, false, 1);
	}
	else if ("conn" == cmd.function) {
		if (cmd.arguments.empty())
			return new InvalidAction(cmd, "must supply target endpoint to make a connection");
		
		int index = parse( cmd.arguments[0] );
		if (index < 0)
			return new InvalidAction(cmd, "`" + cmd.arguments[1] + "` is not a valid index");
		return new NetworkAction(cmd.target, index);
	}
	else if ("attack" == cmd.function) {
		int index = parse( cmd.arguments[0] );
		if (index < 0)
			return new InvalidAction(cmd, "`" + cmd.arguments[1] + "` is not a valid index");
		return new OffensiveAction(cmd.target, index);
	}
	else if ("ping" == cmd.function) {
		int index = parse( cmd.arguments[0] );
		if (index < 0)
			return new InvalidAction(cmd, "`" + cmd.arguments[1] + "` is not a valid index");
		return new CrapAction(cmd.target, index);
	}
	
	return new InvalidAction(cmd, "unrecognized command: `" + cmd.function + "`");
}
*/

ResultSet process_api(Command& cmd)
{
	return { the_game.cmd_api->execute(cmd) };
}

//ResultSet execute(Action& action)
//{
//	return action.execute();
//};

#pragma the game itself

Result test_api_1(Command& cmd)
{
	return list_programs(cmd.target->kernel).front();
}

Result test_api_2(Command& cmd)
{
	return { -1, "NOT IMPLEMENTED" };
}

Result test_api_3(Command& cmd)
{
	std::exit(EXIT_SUCCESS);
	return { 0, "doesn't matter" };
}

Result test_api_4(Command& cmd)
{
	return uninstall_program(cmd.target->kernel, cmd.arguments.back());
}

Result api_not_implemented(Command& cmd)
{
	return { -1, "NOT IMPLEMENTED" };
}


void build_cmd_api()
{
	// create the attack API commands...
	auto msg_ping = branch_node("ping", {
		leaf_node("@c", api_not_implemented)
	});
	auto msg_xfer = branch_node("xfer", {
		leaf_node("@c", api_not_implemented)
	});
	auto msg_cfg = branch_node("cfg", {
		leaf_node("@c", api_not_implemented)
	});
	auto msg_simplex = branch_node("simplex", {
		leaf_node("@c", api_not_implemented)
	});
	auto msg_inf = branch_node("inf", {
		leaf_node("@c", api_not_implemented)
	});
	auto msg = branch_node("msg", {
		msg_ping,
		msg_xfer,
		msg_cfg,
		msg_simplex,
		msg_inf
	});
	
	// create the networking API commands...
	auto net_conn = branch_node("conn", {
		leaf_node("@c", api_not_implemented)
	});
	auto net_whois = branch_node("whois", {
		leaf_node("@c", api_not_implemented)
	});
	auto net_route = branch_node("route", {
		leaf_node("@c", api_not_implemented)
	});
	auto net = branch_node("net", {
		leaf_node("scan", api_not_implemented),
		net_whois,
		net_route,
		net_conn
	});
	
	// create the device config commands...
	auto dev_info = branch_node("info", {
		leaf_node("@h", api_not_implemented)
	});
	auto dev_config = branch_node("config", {
		leaf_node("@h", api_not_implemented)
	});
	auto dev_restore = branch_node("restore", {
		leaf_node("@h", api_not_implemented)
	});
	auto dev = branch_node("dev", {
		leaf_node("list", api_not_implemented),
		dev_info,
		dev_config,
		dev_restore
	});
	
	// create the filesystem commands...
	auto fs_info = branch_node("info", {
		leaf_node("@f", api_not_implemented)
	});
	auto fs_delete = branch_node("delete", {
		leaf_node("@f", api_not_implemented)
	});
	auto fs_copy = branch_node("copy", {
		leaf_node("@f", api_not_implemented)
	});
	auto fs = branch_node("fs", {
		leaf_node("list", api_not_implemented),
		fs_info,
		fs_copy,
		fs_delete
	});
	
	// create the program commands...
	auto ps_info = branch_node("info", {
		leaf_node("@p", test_api_2)
	});
	auto ps_uninstall = branch_node("uninstall", {
		leaf_node("@p", test_api_4)
	});
	auto ps_install = branch_node("install", {
		leaf_node("@p", api_not_implemented)
	});
	auto ps = branch_node("ps", {
		leaf_node("list", test_api_1),
		ps_info,
		ps_install,
		ps_uninstall
	});
	
	// root API map
	the_game.cmd_api = node({
		ps,
		fs,
		dev,
		net,
		msg,
		leaf_node("exit", test_api_3)
	});
}

void build_cmd_gui()
{
	the_game.cmd_gui = command_tree(the_game.cmd_api, the_game.agents.at(0).kernel, the_game.cmd_gui);
}

void load_gamestate()
{
	the_game.current_agent_idx = 0;
	the_game.mode_index = 0;
	the_game.cmd_api_bit = false;
	
	// devices ...
	Auto::Device dev_disk_hitachi =	{ "Hitachi 500", Component::Disk, 100, 16 };
	Auto::Device dev_disk_intel =	{ "Intel 4080", Component::Disk, 100, 16 };
	Auto::Device dev_disk_fujitsu = { "Fujitsu 100HD", Component::Disk, 112, 28 };
	Auto::Device dev_disk_samsung = { "Samsung 1650", Component::Disk, 128, 36 };
	Auto::Device dev_disk_xilinx =	{ "Xilinx 10", Component::Disk, 96, 24 };
	
	Auto::Device dev_mem_crucial =	{ "Crucial 64GB", Component::Memory, 64, 8 };
	Auto::Device dev_mem_kingston = { "Kingston 64i8ab", Component::Memory, 64, 8 };
	Auto::Device dev_mem_msi =		{ "MSI DDR 6k", Component::Memory, 32, 8 };
	Auto::Device dev_mem_intel =	{ "Intel LGA 5700", Component::Memory, 48, 12 };
	Auto::Device dev_mem_trident =	{ "Trident Z Royal Series A", Component::Memory, 32, 10 };
	Auto::Device dev_mem_dell =		{ "Dell HyperX Memristor", Component::Memory, 128, 24 };
	
	Auto::Device dev_cpu_intel_1 =	{ "Intel i7", Component::Processor, 20, 128 };
	Auto::Device dev_cpu_ibm =		{ "IBM Power9", Component::Processor, 32, 112 };
	Auto::Device dev_cpu_amd = 		{ "AMD Ryzen 9 5900x", Component::Processor, 12, 96 };
	Auto::Device dev_cpu_ono_s = 	{ "Ono Sendai 2155", Component::Processor, 32, 128 };
	Auto::Device dev_cpu_intel_2 = 	{ "Intel Xeon W-10855m", Component::Processor, 64, 192 };
	
	Auto::Device dev_net_cisco =	{ "Cisco RT44", Component::Network, 4, 12 };
	Auto::Device dev_net_dec_oc =	{ "DEC OC 768", Component::Network, 4, 12 };
	Auto::Device dev_net_netgear =	{ "Netgear Duo", Component::Network, 2, 8 };
	Auto::Device dev_net_intel =	{ "Intel 82574L Gigabit-E", Component::Network, 8, 10 };
	Auto::Device dev_net_infinib =	{ "Infiniband RDMA Fabric", Component::Network, 12, 16 };
	
	Auto::Device dev_bt_anker =		{ "Anker PowerCore 2100", Component::Battery, 100, 100 };
	Auto::Device dev_bt_aibocn =	{ "Aibocn 10000mAh Power Bank", Component::Battery, 100, 100 };
	Auto::Device dev_bt_amdahl =	{ "Amdahl 8020b", Component::Battery, 100, 100 };
	
	Auto::Device dev_pw_evga =		{ "EVGA M1 PSU", Component::Power, 100, 100 };
	Auto::Device dev_pw_gigabyte =	{ "Gigabyte PSU", Component::Power, 100, 100 };
	Auto::Device dev_pw_corsair =	{ "Corsair CX450 PSU", Component::Power, 100, 100 };
	Auto::Device dev_pw_seasonic =	{ "Seasonic Platinum 650T PSU", Component::Power, 100, 100 };
	Auto::Device dev_pw_sfx =		{ "SFX Lightning PSU", Component::Power, 100, 100 };
	
	// software ...
	Software sw_program_ping =		{ "ping", "ICMP network control", 10, 1, 1, Packet::Ping, Binary::Program, Encryption::Kerberos };
	Software sw_program_cfg =		{ "cfg", "System control", 5, 1, 1, Packet::Symplex, Binary::Program, Encryption::None };
	Software sw_program_inf =		{ "inf", "Inference ctx control", 10, 1, 1, Packet::Inference, Binary::Program, Encryption::Kerberos };
	Software sw_daemon_simplex =	{ "xym", "Symplex backplane", 10, 1, 1, Packet::Symplex, Binary::Daemon, Encryption::None };
	Software sw_daemon_inet =		{ "inet", "syscrtl inode", 5, 1, 1, Packet::Reflection, Binary::Daemon, Encryption::None };
	
	// files ...
	File file_dalle_1 =		{ "Dall-E2", "Model weights for image GAN", "", 4, 100 };
	File file_emails =		{ "Emails", "All my personal emails", "", 0, 4 };
	File file_stack =		{ "local trace", "Global Stack Trace", "", 4, 100 };
	File file_crypto_db =	{ "Crypto DB", "Database containing an index for crypto algos", "", 1, 10 };
	
	
	Computer comp = { "567g8k", "Apple", dev_disk_hitachi, dev_mem_crucial, dev_cpu_ono_s, dev_net_dec_oc, {}, { "Gigabyte", Component::Power, 100, 100 } };
	Kernel system = { "MacOS", { sw_program_ping, sw_program_cfg }, {}, { file_dalle_1, file_emails }, {}, 1, comp, 5 };
	Agent player = { "caleb", "author", Auto::Status::Active, Auto::Class::Player, 1, system };
	the_game.agents.push_back(player);
	
	Computer comp2 = { "nkpasd8", "IBM", dev_disk_intel, dev_mem_kingston, dev_cpu_intel_1, dev_net_cisco, {}, { "Gigabyte", Component::Power, 100, 100 } };
	Kernel system2 = { "OS/2", { sw_program_inf }, { sw_daemon_simplex, sw_daemon_inet }, { file_dalle_1, file_crypto_db }, {}, 1, comp2, 10 };
	Agent ai_player = { "wintermute", "victor", Auto::Status::Active, Auto::Class::Automaton, 1, system2 };
	the_game.agents.push_back(ai_player);
	
	
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
	
	
	the_game.cmd_history = {"    ___         __                        __            ",
							"   /   | __  __/ /_____  ____ ___  ____ _/ /_____  ____ ",
							"  / /| |/ / / / __/ __ \\/ __ `__ \\/ __ `/ __/ __ \\/ __ \\",
							" / ___ / /_/ / /_/ /_/ / / / / / / /_/ / /_/ /_/ / / / /",
							"/_/  |_\\__,_/\\__/\\____/_/ /_/ /_/\\__,_/\\__/\\____/_/ /_/ "};
	
	build_cmd_api();
	build_cmd_gui();
};


bool is_game_over() {
	// is player dead?
	return the_game.agents[0].kernel.hitpoints <= 0;
}


ftxui::Component ModalComponent(std::function<void()> do_nothing, std::function<void()> quit)
{
	using namespace ftxui;
	
	auto bg = AnimatedBackground(ButtonOption::Animated(Color::Aquamarine1, Color::RoyalBlue1));
	auto style = ButtonOption::Animated();
	auto component = Container::Vertical({
		Button("Retry", do_nothing, style),
		Button("Quit", quit, style),
		bg,
	}) | Renderer([&](Element inner) {
		return vbox({
			text("Game Over"),
			separator(),
			inner,
		})
		| borderDouble
		| size(WIDTH, GREATER_THAN, 30)
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
					append_to_history(command); //<!-- we just assume that the computer never makes an ill-formed command
					ResultSet resy = process_api(command);
					if (!resy.empty() && resy.at(0).status != 0)
						append_to_history(resy.at(0).message);
					else
						append_to_history(command);
				}
				else if (agent.type == Class::Player) {
					while (!the_game.cmd_api_bit) {
						using namespace std::chrono_literals;
						std::this_thread::sleep_for(0.1s);
//						the_game.msgr->update();
					}
					/*
					for (Action* axn_ptr : the_game.actions) {
						ResultSet results = execute(*axn_ptr);
						the_game.axn_results = results[0].message;
						build_player_cmd_tree();
						
						screen.Post(ftxui::Event::Custom);	// <!-- triggers a re-render
						
						// log results to file?
					}
					the_game.actions.clear();
					 */
					screen.Post(ftxui::Event::Custom);
					the_game.cmd_api_bit = false;
				}
			}
			game_over = is_game_over();
			the_game.gameover = game_over;
		}
	});
	
	std::string input_str;
	int input_index;
	ftxui::Component input_command;
	auto submit_fn = [&](std::string cmd_str){
		input_str = cmd_str;
		if (!cmd_str.empty()) append_to_history(cmd_str);
		the_game.show_cmd_menu = !the_game.show_cmd_menu;
		input_command->TakeFocus();
	};
	auto cmd_palette = CommandPalette(input_str, submit_fn, the_game.cmd_gui);
	
	bool detail = false;
	auto action = [&] { the_game.show_cmd_menu = !the_game.show_cmd_menu; cmd_palette->TakeFocus(); };
	auto btn = Button("Help", action, ButtonOption::Ascii());
	
	auto modal_component = ModalComponent([]{}, screen.ExitLoopClosure());
	
	
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
	
	auto input_option = ftxui::InputOption();
	input_option.cursor_position = &input_index;
	input_option.on_enter = [&] {
		Command cmd = parse(input_str, &the_game.agents[0]); // <-- GAaah
		Result res = validate(cmd);
		if (res.status == 0) {
			ResultSet resy = process_api(cmd);
			build_cmd_gui();
			if (!resy.empty() && resy.at(0).status != 0)
				append_to_history(resy.at(0).message);
			else
				append_to_history(cmd);
			the_game.cmd_api_bit = true;
		}
		else append_to_history(res.message);
		/*
		if (res.status != 0) {
			append_to_history(res.message);
		}
		else {
			Action* action = process(cmd);
			// CJJ: vestigial!! TODO: update or refactor this section...
			InvalidAction* check_action = dynamic_cast<InvalidAction*>(action);
			bool valid = (check_action == nullptr);
			if (valid) {
				append_to_history(cmd);
				the_game.actions.push_back(action);
			}
			else {
				append_to_history(check_action->msg());
			}
		}
		 */
		
		input_str = "";
	};
	input_command = ftxui::Input(&input_str, "_", input_option);
	
	
	std::vector<std::string> tab_entries = { "Development", "Networking", "Infiltration" };
	auto tab_selection = Menu(&tab_entries, &the_game.mode_index, MenuOption::HorizontalAnimated());
	auto tab_content = Container::Tab({ dev, net, hack }, &the_game.mode_index);
	
	auto main_container = Container::Vertical({
		tab_selection,
		tab_content,
		Container::Vertical({ input_command, btn, animated_field })
	});

	
	auto renderer = Renderer(main_container, [&] {
		Elements cmds;
		for (std::string& cmd : the_game.cmd_history) cmds.push_back(text(cmd));
		
		return vbox({
			text("Automaton") | bold | hcenter,
			tab_selection->Render(),
			tab_content->Render() | flex,
			vbox(cmds) | border,
			separatorHSelector(0, 0, Color::Palette256::Red1, Color::Palette256::SeaGreen1),
			hbox({
				input_command->Render() | flex,
				btn->Render()
			})
		});
	});
	
	renderer |= CatchEvent([&](ftxui::Event event) {
		if (event == ftxui::Event::Tab) {
			Command cmd = parse(input_str, &the_game.agents[0]);
			Result res = autocomplete(cmd);
			if (0 != res.status) return false;
			input_str = res.message;
			input_index = static_cast<int>(input_str.size());
			return true;
		}
		else if (event == ftxui::Event::Escape) {
			the_game.show_cmd_menu = !the_game.show_cmd_menu;
			return true;
		}
		return false;
	});
	
	screen.Loop(renderer | Modal(modal_component, &the_game.gameover) | Layer(cmd_palette, &the_game.show_cmd_menu));
	
	gui.join();
};

}	// namespace Auto

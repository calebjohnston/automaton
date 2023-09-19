//
//  tree.cpp
//
//  Created by Caleb Johnston on 8/23/2023.
//  Copyright © 2023 Caleb Johnston. All rights reserved.
//

#include <algorithm>
#include <deque>
#include <memory>
#include <string>
#include <vector>

#include "tree.hpp"

using namespace std;
using namespace Auto;


bool is_num(const std::string& num)
{
	try {
		int num_arg = std::stoi(num);
		return true;
	}
	catch (...) {
		return false;
	}
}

// TODO: there is a bug if the first argument in a LeafNode needs to be a specific type (program, device, etc)
bool argmatch(const std::string& arg, const std::string& token)
{
	if (arg == token)
		return true;
	else {
		if ("_" == token) {
			return true;
		}
		else if ("*" == token) {
			return !arg.empty();
		}
		else if ("$" == token) {
			return !is_num(arg);
		}
		else if ("#" == token) {
			return is_num(arg);
		}
		else if ("@p" == token ||
				 "@d" == token ||
				 "@f" == token ||
				 "@h" == token ||
				 "@c" == token)
		{
			return !arg.empty();
		}
		else return false;
	}
	
	return false;
}

Node NodeBase::find(const string& str)
{
	auto iter = find_if(begin(children_), end(children_), [&](const Node child) {
		auto cmd = dynamic_pointer_cast<CommandNodeBase>(child);
		if (cmd) {
			return cmd->token() == str;
		}
		else {
			auto leaf = dynamic_pointer_cast<LeafNode>(child);
			if (leaf) {
				return argmatch(str, leaf->token());
			}
			else {
				auto branch = dynamic_pointer_cast<BranchNode>(child);
				if (branch) {
					return branch->token() == str;
				}
			}
		}
		return false;
	});
	return iter == std::end(children_) ? nullptr : *iter;
}

// TODO: simplify this??
Result NodeBase::validate(Command& cmd)
{
	deque<string> cmd_arr = { cmd.function };
	copy(cmd.arguments.begin(), cmd.arguments.end(), back_inserter(cmd_arr));
//	string out;
//	string cmd_str = accumulate(cmd_arr.begin(), cmd_arr.end(), out,
//								[](string lhs, string rhs) {
//									return std::move(lhs) + " " + rhs;
//								});

	// verify that we can trace a tree following a sequence of tokens
	Node tree_node = shared_from_this();
	while (!cmd_arr.empty()) {
		auto str = cmd_arr.front();
		cmd_arr.pop_front();
		tree_node = tree_node->find(str);
//		if (!tree_node) return { -1, "invalid command: " + cmd_str };
		if (!tree_node) return { -1, "invalid command" };
	}
	
	// verify that the final token was a leaf
	auto leaf = dynamic_pointer_cast<LeafNode>(tree_node);
	if (nullptr != leaf)
		return { 0, "Success" };
	else
		return { -1, "invalid command" }; //{ -1, "invalid command: " + cmd_str };
}

Result NodeBase::autocomplete(Command& cmd)
{
	std::string result;
	deque<string> cmd_arr = { cmd.function };
	copy(cmd.arguments.begin(), cmd.arguments.end(), back_inserter(cmd_arr));
	
	Node tree_node = shared_from_this();
	while (!cmd_arr.empty()) {
		auto str = cmd_arr.front();
		cmd_arr.pop_front();
		auto test_node = tree_node->find(str);
		if (!test_node) {
			auto branch_node = std::dynamic_pointer_cast<BranchNode>(tree_node);
			if (branch_node) {
				tree_node = branch_node->partial_match_token(str);
			}
			if (!tree_node)
				return { -1, "no autocomplete available" };
			else {
				branch_node = std::dynamic_pointer_cast<BranchNode>(tree_node);
				if (branch_node) result += branch_node->token();
			}
			cmd_arr.clear();
		}
		else {
			tree_node = test_node;
			result += str + " ";
		}
	}
	
	return { 0, result };
}

// TODO: basically duplicated with NodeBase::validate -- make it DRY!
Result NodeBase::execute(Command& cmd)
{
	deque<string> cmd_arr = { cmd.function };
	copy(cmd.arguments.begin(), cmd.arguments.end(), back_inserter(cmd_arr));
	
	// verify that we can trace a tree following a sequence of tokens
	Node tree_node = shared_from_this();
	while (!cmd_arr.empty()) {
		auto str = cmd_arr.front();
		cmd_arr.pop_front();
		tree_node = tree_node->find(str);
		if (!tree_node) return { -1, "invalid command" };
	}
	
	// verify that the final token was a leaf
	auto leaf = dynamic_pointer_cast<LeafNode>(tree_node);
	if (nullptr != leaf)
		return leaf->execute(cmd);
	else
		return { -1, "invalid command" };
}



//
//  tree.h
//
//  Created by Caleb Johnston on 8/23/2023.
//  Copyright © 2023 Caleb Johnston. All rights reserved.
//

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "model.h"

namespace Auto {

class TreeNode {
public:
	TreeNode(const std::string& str) : _data(str) {}
	TreeNode(const std::string& str, const std::vector<std::string>& kids) : _data(str)
	{
		for (const std::string& kid : kids) _children.push_back(new TreeNode(kid));
		_children_str = kids;
	}
	~TreeNode() {}
	
	std::string data() const { return _data; }
	const std::string& cdata() const { return _data; }
	const std::vector<std::string>& str_children() const { return _children_str; }
	const std::vector<TreeNode*>& children() const { return _children; }

	bool add_child(const std::string& str, const std::vector<std::string>& kids = {}) {
		TreeNode* node = new TreeNode(str, kids);
		_children.push_back(node);
		_children_str.push_back(str);
		return true;
	}
	bool add_child(TreeNode* node) {
		if (node == nullptr) return false;
		_children.push_back(node);
		_children_str.push_back(node->data());
		return true;
	}
	
	TreeNode* child_for_str(const std::string& str) {
		auto iter = std::find_if(std::begin(_children), std::end(_children), [&](const TreeNode* node){
			return node->data() == str;
		});
		return iter == std::end(_children) ? nullptr : *iter;
	}
	TreeNode* child_for_partial_str(const std::string& str) {
		auto iter = std::find_if(std::begin(_children), std::end(_children), [&](const TreeNode* node){
			return node->data().find(str) == 0;
		});
		return iter == std::end(_children) ? nullptr : *iter;
	}
	TreeNode* child_for_index(const int index) {
		if (index < 0 || index >= _children.size()) return nullptr;
		return _children[index];
	}
	
	
	bool remove_child_str(const std::string& str) {
		auto iter = std::find_if(std::begin(_children), std::end(_children), [&](const TreeNode* node){
			return node->data() == str;
		});
		if (iter == std::end(_children)) return false;
		
		_children.erase(iter);
		return true;
	}
	bool remove_child(const int index) {
		if (index < 0 || index >= _children.size()) return false;
		_children.erase(_children.begin()+index);
		return true;
	}
	
	void clear() {
		_children.clear();
	}
	
private:
	std::string	_data;
	std::vector<TreeNode*> _children;
	std::vector<std::string> _children_str;
};

class NodeBase;
using Node = std::shared_ptr<NodeBase>;
using Nodes = std::vector<Node>;

class NodeBase : public std::enable_shared_from_this<NodeBase> {
public:
	NodeBase() = default;
	NodeBase(Nodes children) : children_(std::move(children)) {};
	NodeBase(const NodeBase&) = delete;
	NodeBase(const NodeBase&&) = delete;
	NodeBase& operator=(const NodeBase&) = delete;
	NodeBase& operator=(const NodeBase&&) = delete;
	virtual ~NodeBase() = default;
	
	Node find(const std::string& str);
	virtual Result validate(Command& cmd);
	virtual Result autocomplete(Command& cmd);
	virtual Result execute(Command& cmd);
	const Nodes& children() const { return children_; }
	
protected:
	Nodes children_;
};

class BranchNode : public NodeBase {
public:
	explicit BranchNode(std::string& token, Nodes children) : NodeBase(std::move(children)), token_(token) {}
	
	std::string token() const { return token_; }
	
	Node partial_match_token(const std::string& str) const {
		auto iter = std::find_if(std::begin(children_), std::end(children_), [&](const Node child){
			auto cmd_node = std::dynamic_pointer_cast<BranchNode>(child);
			if (!cmd_node) throw;
			return cmd_node->token().find(str) == 0;
		});
		return iter == std::end(children_) ? nullptr : std::dynamic_pointer_cast<BranchNode>(*iter);
	}
	
protected:
	std::string	token_;
};

class LeafNode : public BranchNode {
public:
	explicit LeafNode(std::string& token, std::function<Result(Command&)> api)
		: BranchNode(token, {}), api_(api) {}
	
	virtual Result execute(Command& cmd) override { return api_(cmd); }
	
private:
	std::function<Result(Command&)> api_;
};

class CommandNodeBase;
using CommandNode = std::shared_ptr<CommandNodeBase>;
using CommandNodes = std::vector<CommandNode>;

class CommandNodeBase : public BranchNode {
public:
	explicit CommandNodeBase(std::string& token, Nodes kids) : BranchNode(token, kids) {
		for (int i=0; i< this->children_.size(); i++) {
			auto cmd_node = std::dynamic_pointer_cast<CommandNodeBase>(this->children_[i]);
			if (!cmd_node) throw;
			children_str_.push_back(cmd_node->token());
		}
	}
	
	void reset(std::string token, Nodes kids) {
		children_.clear();
		children_str_.clear();
		token_ = token;
		children_ = kids;
		for (auto child : children_) {
			auto cmd_node = std::dynamic_pointer_cast<CommandNodeBase>(child);
			if (!cmd_node) throw;
			children_str_.push_back(cmd_node->token());
		}
	}
	
	const std::vector<std::string>& str_children() const { return children_str_; }
	
	CommandNode child_for_index(const int index) {
		if (index < 0 || index >= children_.size()) return nullptr;
		return std::dynamic_pointer_cast<CommandNodeBase>(children_.at(index));
	}

	CommandNode child_for_str(const std::string& str) const {
		auto iter = std::find_if(std::begin(children_), std::end(children_), [&](const Node child){
			auto cmd_node = std::dynamic_pointer_cast<CommandNodeBase>(child);
			if (!cmd_node) throw;
			return cmd_node->token() == str;
		});
		return iter == std::end(children_) ? nullptr : std::dynamic_pointer_cast<CommandNodeBase>(*iter);
	}
	
	void clear() {
		children_.clear();
	}

private:
	std::vector<std::string> children_str_;
};

static Node node(Nodes children)
{
	return std::make_shared<NodeBase>(std::move(children));
}

static Node branch_node(std::string token, Nodes children)
{
	return std::make_shared<BranchNode>(token, std::move(children));
}

static Node leaf_node(std::string token, std::function<Result(Auto::Command&)> api_fn)
{
	return std::make_shared<LeafNode>(token, api_fn);
}

static CommandNode command_node(std::string token, Nodes children)
{
	return std::make_shared<CommandNodeBase>(token, children);
}

// TODO: this function is very not DRY -- refactor...
static CommandNode command_tree(Node node, Kernel& player, CommandNode root_node)
{
	Nodes cmds;
	for (const Node& child : node->children()) {
		auto leaf = std::dynamic_pointer_cast<LeafNode>(child);
		if (leaf) {
			if ("@p" == leaf->token()) {
				auto list = program_names(player);
				for (auto item : list) cmds.push_back(command_node(item, {}));
				if (list.empty()) cmds.push_back(command_node("{program}", {}));
			}
			else if ("@d" == leaf->token()) {
				auto list = daemon_names(player);
				for (auto item : list) cmds.push_back(command_node(item, {}));
				if (list.empty()) cmds.push_back(command_node("{daemon}", {}));
			}
			else if ("@f" == leaf->token()) {
				auto list = file_names(player);
				for (auto item : list) cmds.push_back(command_node(item, {}));
				if (list.empty()) cmds.push_back(command_node("{file}", {}));
			}
			else if ("@h" == leaf->token()) {
				auto list = device_names(player);
				for (auto item : list) cmds.push_back(command_node(item, {}));
				if (list.empty()) cmds.push_back(command_node("{device}", {}));
			}
			else if ("@c" == leaf->token()) {
				auto list = connection_names(player);
				for (auto item : list) cmds.push_back(command_node(item, {}));
				if (list.empty()) cmds.push_back(command_node("{connection}", {}));
			}
			else {
				cmds.push_back(command_node(leaf->token(), {}));
			}
		}
		else {
			auto branch = std::dynamic_pointer_cast<BranchNode>(child);
			if (branch) {
				cmds.push_back(command_tree(branch, player, root_node));
			}
			else throw;
		}
	}
	
	auto leaf = std::dynamic_pointer_cast<LeafNode>(node);
	auto branch = std::dynamic_pointer_cast<BranchNode>(node);
	if (leaf) {
		if ("@p" == leaf->token()) {
			auto list = program_names(player);
			for (auto item : list) cmds.push_back(command_node(item, {}));
			if (list.empty()) cmds.push_back(command_node("{program}", {}));
		}
		else if ("@d" == leaf->token()) {
			auto list = daemon_names(player);
			for (auto item : list) cmds.push_back(command_node(item, {}));
			if (list.empty()) cmds.push_back(command_node("{daemon}", {}));
		}
		else if ("@f" == leaf->token()) {
			auto list = file_names(player);
			for (auto item : list) cmds.push_back(command_node(item, {}));
			if (list.empty()) cmds.push_back(command_node("{file}", {}));
		}
		else if ("@h" == leaf->token()) {
			auto list = device_names(player);
			for (auto item : list) cmds.push_back(command_node(item, {}));
			if (list.empty()) cmds.push_back(command_node("{device}", {}));
		}
		else if ("@c" == leaf->token()) {
			auto list = connection_names(player);
			for (auto item : list) cmds.push_back(command_node(item, {}));
			if (list.empty()) cmds.push_back(command_node("{connection}", {}));
		}
		else {
			return command_node(leaf->token(), {});
		}
	}
	else if (branch) return command_node(branch->token(), cmds);
	else if (!root_node) return command_node("{root}", cmds);
	else {
		root_node->reset("{root}", cmds);
		return root_node;
	}
}

}	// namespace Auto

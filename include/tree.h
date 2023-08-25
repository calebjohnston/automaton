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

#include "game.h"

namespace Auto {
/*
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
	std::vector<std::string> _children_str; //<!-- TODO: move to the dynamic tree
};
*/

class NodeBase;
using Node = std::shared_ptr<NodeBase>;
using Nodes = std::vector<Node>;

class NodeBase {
public:
	NodeBase() = default;
	NodeBase(Nodes children) : children_(std::move(children)) {};
	NodeBase(const NodeBase&) = delete;
	NodeBase(const NodeBase&&) = delete;
	NodeBase& operator=(const NodeBase&) = delete;
	NodeBase& operator=(const NodeBase&&) = delete;
	virtual ~NodeBase() = default;
	
protected:
	Nodes children_;
};

class BranchNode : public NodeBase {
public:
	explicit BranchNode(std::string& token, Nodes children) : NodeBase(std::move(children)), token_(token) {}
	
protected:
	std::string	token_;
};

class LeafNode : public NodeBase {
public:
	explicit LeafNode(std::vector<std::string>& args, std::function<Result(Command&)> api)
	: NodeBase(), args_(args), api_(api) {}
	
private:
	std::vector<std::string> args_;
	std::function<Result(Command&)> api_;
};

//using CommandNode = std::shared_ptr<CommandNodeBase>;
//using CommandNodes = std::vector<CommandNode>;

class CommandNode : public BranchNode {
public:
	explicit CommandNode(std::string& token, Nodes children) : BranchNode(token, std::move(children)) {
		for (const Node& child : children_) children_.push_back(child->token_);
	}
	
	const std::vector<std::string>& str_children() const { return children_str_; }
	
	CommandNode child_for_str(const std::string& str) const {
		auto iter = std::find_if(std::begin(children_), std::end(children_), [&](const CommandNode node){
			return node->token_ == str;
		});
		return iter == std::end(children_) ? nullptr : *iter;
	}
	CommandNode child_for_partial_str(const std::string& str) const {
		auto iter = std::find_if(std::begin(children_), std::end(children_), [&](const CommandNode node){
			return node->token_.find(str) == 0;
		});
		return iter == std::end(children_) ? nullptr : *iter;
	}

private:
	std::vector<std::string> children_str_;
};

}	// namespace Auto

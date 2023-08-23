//
//  tree.hpp
//
//  Created by Caleb Johnston on 8/21/2023.
//  Copyright © 2023 Caleb Johnston. All rights reserved.
//

#pragma once

#include <string>
#include <vector>

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
	TreeNode* _parent;
	std::vector<TreeNode*> _children;
	std::vector<std::string> _children_str;
};

}	// namespace Auto

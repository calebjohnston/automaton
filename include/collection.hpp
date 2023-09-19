//
//  collection.h
//
//  Created by Caleb Johnston on 9/6/2023.
//  Copyright © 2023 Caleb Johnston. All rights reserved.
//

#pragma once

#include <array>
#include <iostream>
#include <map>
#include <vector>
#include <utility>

#include "model.h"

namespace Auto {

/**
 * struct sprite_tag{};
 * typedef ID<sprite_tag, SpriteImpl*, nullptr> sprite_id;
 */

template<class Tag, typename T, T default_value>
class ID {
public:
	static ID invalid() { return ID(); }
	
	// Defaults to ID::invalid()
	ID() : _val(default_value) { }
	
	// Explicit constructor:
	explicit ID(T val) : _val(val) { }
	
	// Explicit conversion to get back the T:
	T value() const { return _val; }
	explicit operator T() const { return _val; }
	
	friend bool operator==(ID a, ID b) { return a.m_val == b._val; }
	friend bool operator!=(ID a, ID b) { return a.m_val != b._val; }
	
private:
	T _val;
};

template<class Tag, typename T>
class Collection {
public:
	typedef ID<Tag, T, nullptr> Handle;
	
public:
	Handle add(T new_val) {
		_storage.push_back(new_val);
		Handle handle_id(new_val);
		return handle_id;
	}
	
	Handle get(size_t index) const {
		if (index >= _storage.size()) return nullptr;
		
		Handle handle_id(_storage.at(index));
		return handle_id;
	}
	
	bool remove(size_t index) {
		if (index >= _storage.size()) return false;
		
		_storage.erase(_storage.begin() + index);
		
		return true;
	}
	
	bool remove(Handle handle) {
		auto iter = std::find(_storage.begin(), _storage.end(), handle.value());
		if (iter == _storage.end())
			return false;
		
		_storage.erase(iter);
		
		return true;
	}
	
	size_t size() const {
		return _storage.size();
	}
	
private:
	std::vector<T> _storage;
};

struct program {};
typedef Collection<program, Software*> ProgramCollection;

}

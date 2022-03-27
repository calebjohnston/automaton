//
//  computer.h
//
//  Created by Caleb Johnston on 3/8/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#pragma once

#include <memory>
#include <string>
#include <stdlib.h>

#include "component.h"
#include "node.h"

typedef std::shared_ptr<class Computer> ComputerRef;

struct ComputerAttribs {
	int power = 0;
	std::string name = "";
	std::string serial = "";
	std::string manufacturer = "";
};

class Computer : public GridNode {
public:
	static ComputerRef create(const ComputerAttribs& attribs);

	std::string serial() const { return _serial; }
	std::string manufacturer() const { return _manufacturer; }

	virtual int add(GridNodeRef child) override;

	virtual bool remove(size_t index) override;

	void disk(ComponentRef device) 		{ _children[_diskIdx] = device; }
	void memory(ComponentRef device) 	{ _children[_memoryIdx] = device; }
	void processor(ComponentRef device) { _children[_processorIdx] = device; }
	void uplink(ComponentRef device) 	{ _children[_uplinkIdx] = device; }
	void battery(DeviceRef device) 		{ _children[_batteryIdx] = device; }
	void powerUnit(DeviceRef device) 	{ _children[_powerIdx] = device; }

	ComponentRef disk() const 		{ return std::dynamic_pointer_cast<Component>(_children[_diskIdx]); }
	ComponentRef memory() const		{ return std::dynamic_pointer_cast<Component>(_children[_memoryIdx]); }
	ComponentRef processor() const	{ return std::dynamic_pointer_cast<Component>(_children[_processorIdx]); }
	ComponentRef uplink() const		{ return std::dynamic_pointer_cast<Component>(_children[_uplinkIdx]); }
	DeviceRef battery() const		{ return std::dynamic_pointer_cast<Device>(_children[_batteryIdx]); }
	DeviceRef powerUnit() const		{ return std::dynamic_pointer_cast<Device>(_children[_powerIdx]); }

private:
	Computer(const ComputerAttribs& attribs);

	std::string _serial;
	std::string _manufacturer;

	const size_t _diskIdx = 0;
	const size_t _memoryIdx = 1;
	const size_t _processorIdx = 2;
	const size_t _uplinkIdx = 3;
	const size_t _batteryIdx = 4;
	const size_t _powerIdx = 5;
	const size_t _maxChildren = 6;
};

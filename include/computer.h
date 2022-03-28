//
//  computer.h
//
//  Created by Caleb Johnston on 3/8/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#pragma once

#include <map>
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

	void disk(ComponentRef device) 		{ _children[DiskIdx] = device; }
	void memory(ComponentRef device) 	{ _children[MemoryIdx] = device; }
	void processor(ComponentRef device) { _children[ProcessorIdx] = device; }
	void uplink(ComponentRef device) 	{ _children[UplinkIdx] = device; }
	void battery(DeviceRef device) 		{ _children[BatteryIdx] = device; }
	void powerUnit(DeviceRef device) 	{ _children[PowerIdx] = device; }

	ComponentRef disk() const 		{ return std::dynamic_pointer_cast<Component>(_children[DiskIdx]); }
	ComponentRef memory() const		{ return std::dynamic_pointer_cast<Component>(_children[MemoryIdx]); }
	ComponentRef processor() const	{ return std::dynamic_pointer_cast<Component>(_children[ProcessorIdx]); }
	ComponentRef uplink() const		{ return std::dynamic_pointer_cast<Component>(_children[UplinkIdx]); }
	DeviceRef battery() const		{ return std::dynamic_pointer_cast<Device>(_children[BatteryIdx]); }
	DeviceRef powerUnit() const		{ return std::dynamic_pointer_cast<Device>(_children[PowerIdx]); }

private:
	Computer(const ComputerAttribs& attribs);

	std::string _serial;
	std::string _manufacturer;

	const static size_t DiskIdx = 0;
	const static size_t MemoryIdx = 1;
	const static size_t ProcessorIdx = 2;
	const static size_t UplinkIdx = 3;
	const static size_t BatteryIdx = 4;
	const static size_t PowerIdx = 5;
	const static size_t MaxChildren = 6;

	const static std::map<Type,int> ComponentIndexMap;
};

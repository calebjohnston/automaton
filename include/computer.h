//
//  computer.h
//
//  Created by Caleb Johnston on 3/8/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#pragma once

#include <array>
#include <map>
#include <memory>
#include <string>
#include <stdlib.h>

#include "component.h"

typedef std::shared_ptr<class Computer> ComputerRef;

class Computer {
public:
	std::string name;
	std::string serial;
	std::string manufacturer;
	
	Component storage;
	Component memory;
	Component processor;
	Component uplink;
	Device battery;
	Device power;
};


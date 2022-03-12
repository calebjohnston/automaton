//
//  computer.h
//
//  Created by Caleb Johnston on 3/8/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#pragma once

#include <array>
#include <map>
#include <string>
#include <stdlib.h>

#include "component.hpp"

class Computer {
public:
	Component disk;
	Component memory;
	Component processor;
	Component network;
	Device battery;
	Device power;
};


//
//  kernel.h
//
//  Created by Caleb Johnston on 3/8/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#pragma once

#include <string>
#include <vector>

#include "program.hpp"


class Kernel {
public:
	std::string name;
	std::string description;
	std::vector<Daemon> daemons;
	std::vector<Program> programs;
	std::vector<Data> filesystem;
};

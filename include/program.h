//
//  program.h
//
//  Created by Caleb Johnston on 3/8/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#pragma once

#include <string>
#include <vector>

#include "data.h"

typedef std::shared_ptr<class Software> SoftwareRef;
typedef std::shared_ptr<class Program> ProgramRef;
typedef std::shared_ptr<class Daemon> DaemonRef;

class Software : public Data {
public:
	DataPacket type;
	int version;
	
	Software(const DataAttribs attribs) : Data(attribs), type(attribs.type), version(attribs.version) {};
};

class Program : public Software {
public:
	int cycles;
	Encryption crypto;
	std::string command;	// used to match the CLI input
	
	Program(const DataAttribs attribs) : Software(attribs), crypto(attribs.crypto), cycles(attribs.cycles), command(command) {};
};

class Daemon : public Software {
public:
	float efficiency;
	
	Daemon(const DataAttribs attribs) : Software(attribs), efficiency(attribs.efficiency) {};
};


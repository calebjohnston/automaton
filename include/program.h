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
	DataPacket type() const { return _type; }
	int version() const { return _version; }

protected:
	DataPacket _type;
	int _version;

	Software(const DataAttribs attribs) : Data(attribs), _type(attribs.type), _version(attribs.version) {};
};


class Program : public Software {
public:
	static ProgramRef create(const DataAttribs& attribs);

	int cycles() const { return _cycles; }

	Encryption crypto() const { return _crypto; }

	std::string command() const { return _command; }

protected:
	Program(const DataAttribs& attribs) : Software(attribs), _crypto(attribs.crypto), _cycles(attribs.cycles), _command(attribs.command) {};

	int _cycles;
	Encryption _crypto;
	std::string _command;	// used to match the CLI input
};

class Daemon : public Software {
public:
	static DaemonRef create(const DataAttribs& attribs);

	float efficiency() const { return _efficiency; }

protected:
	Daemon(const DataAttribs& attribs) : Software(attribs), _efficiency(attribs.efficiency) {};

	float _efficiency;
};

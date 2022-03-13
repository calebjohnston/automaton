//
//  data.h
//
//  Created by Caleb Johnston on 3/8/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#pragma once

#include <string>
#include <vector>
#include <cstddef>
#include <bitset>

typedef std::shared_ptr<class Data> DataRef;

enum class DataPacket {
	Ping,
	Inference,
	Reflection,
	Symplex,
	Autoregression,
	None
};
enum class Encryption {
	DH4,
	X509,
	Kerberos,
	None
};

struct DataAttribs {
	std::string name = "";
	std::string description = "";
	int size = 0;
	std::byte access = {};
	std::string contents = "";
	DataPacket type = DataPacket::None;
	int version = 0;
	std::string command = "";
	Encryption crypto = Encryption::None;
	int cycles = 0;
	float efficiency = 0;
};

class Data {
public:
	std::string name;
	std::string description;
	int size;
	std::byte access; // revise...

	Data(const DataAttribs& attribs) : name(attribs.name), description(attribs.description), size(attribs.size), access(attribs.access) {};
	
};

class File : public Data {
public:
	std::string contents;
	
	File(const DataAttribs& attribs) : Data(attribs), contents(attribs.contents) {};
};

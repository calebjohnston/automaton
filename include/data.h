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

#include "node.h"

typedef std::shared_ptr<class Data> DataRef;
typedef std::shared_ptr<class File> FileRef;

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

class Data : public GraphNode {
public:
	static DataRef create(const DataAttribs& attribs);

	virtual int add(GraphNodeRef child) override { return -1; }

	virtual bool remove(size_t index) override { return false; }

	std::string description() const { return _description; }

	int size() const { return _size; }

	std::byte access() const { return _access; }

protected:
	Data(const DataAttribs& attribs) : GraphNode({}, attribs.name), _description(attribs.description), _size(attribs.size), _access(attribs.access) {};

	std::string _description;
	int _size;
	std::byte _access; // revise...
};


class File : public Data {
public:
	static FileRef create(const DataAttribs& attribs);

	std::string contents() const { return _contents; }

protected:
	File(const DataAttribs& attribs) : Data(attribs), _contents(attribs.contents) {};

	std::string _contents;
};

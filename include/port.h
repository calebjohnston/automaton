#pragma once

#include <string>

typedef enum port_io_t {
	PORT_NONE,
	PORT_INPUT,
	PORT_OUTPUT
} PortIoType;

class Port
{
public:
    Port();

	std::string getAddress() const { return mAddress; }

	void setAddress(const std::string& addr);

private:
	std::string mAddress;
	PortIoType mIoType;

};

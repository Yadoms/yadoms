#pragma once
#include "Command.h"

class IRealScreen
{
public:
	virtual ~IRealScreen() {}

	virtual void setup_ip(const std::string& ip) = 0;
	virtual void start() = 0;
	virtual void update(const std::vector<CCommand> & cmds) = 0;
	virtual void finalize() = 0;
};

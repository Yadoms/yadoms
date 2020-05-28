#pragma once

#include "Command.h"


class CCommandParser
{

public:
	static bool Parse(const std::string& screenCommands, std::vector<CCommand>& outvec);
};
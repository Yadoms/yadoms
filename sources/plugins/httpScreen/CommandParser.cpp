#include "stdafx.h"
#include "CommandParser.h"
#include "shared/DataContainer.h"

bool CCommandParser::Parse(const std::string& screenCommands, std::vector<CCommand> & outvec)
{
	try
	{
		shared::CDataContainer parsing(screenCommands);

		if(parsing.containsChildArray(""))
		{
			outvec.clear();
			auto cmds = parsing.get<std::vector<std::string>>("");
			for (auto cmd : cmds)
			{
			   if(boost::istarts_with(cmd, "ctrl,"))
			   {
					outvec.push_back(CCommand(true, cmd.substr(5)));
			   }
				else
				{
					outvec.push_back(CCommand(false, cmd));
				}
			}
			return true;
		}

		
	}
	catch(...)
	{
	   
	}
	return false;
}

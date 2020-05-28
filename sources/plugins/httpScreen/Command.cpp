#include "stdafx.h"
#include "Command.h"

CCommand::CCommand(bool cmd, const std::string & data)
	   :m_control(cmd), m_content(data)
{
	
}

CCommand::~CCommand()
{
	
}

bool CCommand::isControl()
{
   return m_control;
}

const std::string & CCommand::get()
{
   return m_content;
}
#include "stdafx.h"
#include "RunScript.h"

namespace job { namespace action
{

CRunScript::CRunScript(const shared::CDataContainer& configuration)
   :m_scriptPath(configuration.get<std::string>("scriptPath"))
{
}

CRunScript::~CRunScript()
{         
}

void CRunScript::doAction() const
{
   //TODO
}

} } // namespace job::action	
	
	
#include "stdafx.h"
#include "RunScript.h"

namespace job { namespace action
{

CRunScript::CRunScript(const shared::CDataContainer& configuration, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway)
   :m_pluginGateway(pluginGateway), m_scriptPath("myScript")//TODO valeur en dur
{
   // Read configuration
   //TODO
}

CRunScript::~CRunScript()
{         
}

void CRunScript::doAction() const
{
   //TODO
}

} } // namespace job::action	
	
	
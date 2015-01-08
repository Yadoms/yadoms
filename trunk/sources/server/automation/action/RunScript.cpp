#include "stdafx.h"
#include "RunScript.h"

namespace automation { namespace action
{

CRunScript::CRunScript(const shared::CDataContainer& configuration, boost::shared_ptr<IScriptRunnerFactory> scriptRunnerFactory)
   :m_scriptRunnerFactory(scriptRunnerFactory),
   m_scriptPath(configuration.get<std::string>("scriptPath")),
   m_scriptConfiguration(configuration.hasValue("") ? configuration.get<shared::CDataContainer>("scriptConfiguration") : shared::CDataContainer())
{
}

CRunScript::~CRunScript()
{         
}

void CRunScript::doAction() const
{
   m_scriptRunnerFactory->createScriptRunner(m_scriptPath, m_scriptConfiguration);
}

} } // namespace automation::action	
	
	
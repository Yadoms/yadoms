#include "stdafx.h"
#include "RunScript.h"
#include <shared/Log.h>
#include "script/YScriptApiImplementation.h"

namespace automation { namespace action
{

CRunScript::CRunScript(const shared::CDataContainer& configuration, boost::shared_ptr<script::IRunnerFactory> scriptRunnerFactory)
   :m_scriptRunnerFactory(scriptRunnerFactory),
   m_scriptPath((boost::filesystem::path(std::string("scripts")) / boost::filesystem::path(configuration.get<std::string>("scriptPath"))).string()),
   m_scriptConfiguration(configuration.hasValue("scriptConfiguration") ? configuration.get<shared::CDataContainer>("scriptConfiguration") : shared::CDataContainer())
{
}

CRunScript::~CRunScript()
{         
}

void CRunScript::doAction() const
{
   boost::shared_ptr<shared::script::IRunner> runner(m_scriptRunnerFactory->createScriptRunner(m_scriptPath, m_scriptConfiguration));

   script::CYScriptApiImplementation context;
   runner->run(context);

   if (!runner->isOk())
   {
      YADOMS_LOG(error) << m_scriptPath << " exit with error : " << runner->error();
   }
}

} } // namespace automation::action	
	
	
#include "stdafx.h"
#include "RunScript.h"
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include "script/YScriptApiImplementation.h"

namespace automation { namespace action
{

CRunScript::CRunScript(const shared::CDataContainer& configuration, boost::shared_ptr<script::IRunnerFactory> scriptRunnerFactory)
   :m_scriptRunnerFactory(scriptRunnerFactory),
   m_scriptName(configuration.get<std::string>("scriptName")),
   m_scriptConfiguration(configuration.hasValue("scriptConfiguration") ? configuration.get<shared::CDataContainer>("scriptConfiguration") : shared::CDataContainer())
{
}

CRunScript::~CRunScript()
{         
}

void CRunScript::doAction()
{
   try
   {
      m_runner = m_scriptRunnerFactory->createScriptRunner(m_scriptName, m_scriptConfiguration);

      script::CYScriptApiImplementation context;
      m_runner->run(context);

      if (!m_runner->isOk())
      {
         YADOMS_LOG(error) << m_scriptName << " exit with error : " << m_runner->error();
      }
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Unable to do action : " << e.what();
   }
}

void CRunScript::stopAction()
{
   m_runner->stop();
}

} } // namespace automation::action	
	
	
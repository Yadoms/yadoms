#include "stdafx.h"
#include "RunScript.h"
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>

namespace automation { namespace action
{

CRunScript::CRunScript(const shared::CDataContainer& configuration, boost::shared_ptr<script::IFactory> scriptFactory)
   :m_scriptFactory(scriptFactory),
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
      m_runner = m_scriptFactory->createScriptRunner(m_scriptName, m_scriptConfiguration);

      boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> context = m_scriptFactory->createScriptContext();
      m_runner->run(*context);

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
   if (!!m_runner)
      m_runner->stop();
}

} } // namespace automation::action	
	
	
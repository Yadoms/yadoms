#include "stdafx.h"
#include "Rule.h"
#include <shared/Log.h>
#include "script/IProperties.h"

namespace automation
{

CRule::CRule(boost::shared_ptr<const database::entities::CRule> ruleData,
   boost::shared_ptr<script::IManager> scriptManager, boost::shared_ptr<IRuleStateHandler> ruleStateHandler)
   :m_ruleData(ruleData),
   m_scriptManager(scriptManager),
   m_ruleStateHandler(ruleStateHandler)
{
   start();
}

CRule::~CRule()
{
}

void CRule::start()
{
   if (!!m_runner)
   {
      YADOMS_LOG(warning) << "Can not start rule " << m_ruleData->Name() << " : already started";
      return;
   }

   boost::shared_ptr<script::IProperties> scriptProperties = m_scriptManager->createScriptProperties(m_ruleData);
   boost::shared_ptr<shared::process::ILogger> scriptLogger = m_scriptManager->createScriptLogger(scriptProperties->scriptPath());
   boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi = m_scriptManager->createScriptContext(scriptLogger);
   boost::shared_ptr<shared::process::IStopNotifier> stopNotifier = m_scriptManager->createStopNotifier(m_ruleStateHandler, m_ruleData->Id());

   m_runner = m_scriptManager->createScriptRunner(scriptProperties, scriptLogger, yScriptApi, stopNotifier);
}

void CRule::requestStop()
{
//TODO corriger   m_runner->requestStop();
}

} // namespace automation	
	
	
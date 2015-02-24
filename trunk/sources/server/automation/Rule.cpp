#include "stdafx.h"
#include "Rule.h"
#include <shared/Log.h>
#include "script/IProperties.h"

namespace automation
{

CRule::CRule(boost::shared_ptr<const database::entities::CRule> ruleData,
   boost::shared_ptr<script::IFactory> scriptFactory, boost::shared_ptr<IRuleErrorHandler> ruleErrorHandler)
   :m_ruleData(ruleData),
   m_scriptFactory(scriptFactory),
   m_ruleErrorHandler(ruleErrorHandler)
{
}

CRule::~CRule()
{
   stop();
}

void CRule::start()
{
   m_thread.reset(new CRuleThread(m_ruleData->Name(), *this));
   m_thread->start();
}

void CRule::stop()
{
   if (!!m_runner)
      m_runner->stop();
   m_thread->stop();
   m_thread.reset();
}

void CRule::doWork()
{
   try
   {
      boost::shared_ptr<script::IProperties> scriptProperties = m_scriptFactory->createScriptProperties(m_ruleData);
      boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> context = m_scriptFactory->createScriptContext();

      // Loop on the script
      do
      {
         m_runner = m_scriptFactory->createScriptRunner(scriptProperties);
         m_runner->run(*context);
         boost::this_thread::interruption_point();
      } while (m_runner->isOk());

      m_ruleErrorHandler->signalRuleError(m_ruleData->Id(), (boost::format("%1% exit with error : %2%") % m_ruleData->Name() % m_runner->error()).str());
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      m_ruleErrorHandler->signalRuleError(m_ruleData->Id(), (boost::format("%1% : Unable to do action : %2%") % m_ruleData->Name() % e.what()).str());
   }
   catch (boost::thread_interrupted&)
   {
      m_ruleErrorHandler->signalNormalRuleStop(m_ruleData->Id());
   }
}

} // namespace automation	
	
	
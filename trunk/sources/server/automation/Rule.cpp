#include "stdafx.h"
#include "Rule.h"
#include <shared/Log.h>
#include "script/IProperties.h"

namespace automation
{

const boost::chrono::milliseconds CRule::m_MinRuleDuration(1000);

CRule::CRule(boost::shared_ptr<const database::entities::CRule> ruleData,
   boost::shared_ptr<script::IFactory> scriptFactory, boost::shared_ptr<IRuleStateHandler> ruleStateHandler)
   :m_ruleData(ruleData),
   m_scriptFactory(scriptFactory),
   m_ruleStateHandler(ruleStateHandler)
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

      // Loop on the script.
      // If a rule takes less than m_MinRuleDuration, wait for the competing duration so that a rule
      // can not take 100% CPU.
      do
      {
         m_runner.reset();
         m_runner = m_scriptFactory->createScriptRunner(scriptProperties);

         boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();

         m_runner->run(*context);

         boost::chrono::system_clock::time_point end = boost::chrono::system_clock::now();

         boost::chrono::milliseconds ruleDuration = boost::chrono::duration_cast<boost::chrono::milliseconds>(end - start);
         if (ruleDuration < m_MinRuleDuration)
            boost::this_thread::sleep_for(m_MinRuleDuration - ruleDuration);

         boost::this_thread::interruption_point();
      } while (m_runner->isOk());

      m_ruleStateHandler->signalRuleError(m_ruleData->Id(), (boost::format("%1% exit with error : %2%") % m_ruleData->Name() % m_runner->error()).str());
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      m_ruleStateHandler->signalRuleError(m_ruleData->Id(), (boost::format("%1% : Unable to do action : %2%") % m_ruleData->Name() % e.what()).str());
   }
   catch (boost::thread_interrupted&)
   {
      m_ruleStateHandler->signalNormalRuleStop(m_ruleData->Id());
   }
}

} // namespace automation	
	
	
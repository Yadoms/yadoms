#include "stdafx.h"
#include "Rule.h"
#include <shared/Log.h>
#include "script/IProperties.h"

namespace automation
{

CRule::CRule(boost::shared_ptr<const database::entities::CRule> ruleData,
   boost::shared_ptr<script::IFactory> scriptFactory)
   :m_ruleData(ruleData),
   m_scriptFactory(scriptFactory)
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

      m_runner = m_scriptFactory->createScriptRunner(scriptProperties);

      boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> context = m_scriptFactory->createScriptContext();
      m_runner->run(*context);

      if (!m_runner->isOk())
      {
         YADOMS_LOG(error) << m_ruleData->Name() << " exit with error : " << m_runner->error();
      }
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Unable to do action : " << e.what();
   }
   catch (boost::thread_interrupted&)
   {
   }
}

} // namespace automation	
	
	
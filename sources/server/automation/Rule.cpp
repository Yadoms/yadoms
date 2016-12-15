#include "stdafx.h"
#include "Rule.h"
#include <shared/Log.h>
#include "script/IProperties.h"
#include "script/ScriptLogConfiguration.h"
namespace automation
{
   CRule::CRule(boost::shared_ptr<const database::entities::CRule> ruleData,
                boost::shared_ptr<script::IManager> scriptManager,
                boost::shared_ptr<IRuleStateHandler> ruleStateHandler)
      : m_ruleData(ruleData),
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
      if (!!m_process)
      {
         YADOMS_LOG(warning) << "Can not start rule " << m_ruleData->Name() << " : already started";
         return;
      }
      try
      {
         auto scriptProperties = m_scriptManager->createScriptProperties(m_ruleData);

         script::CScriptLogConfiguration config;
         boost::filesystem::path file = m_scriptManager->getScriptLogFileName(m_ruleData);
         const std::string loggerName = "script/" + m_ruleData->Name() + " #" + std::to_string(m_ruleData->Id());
         Poco::Logger & scriptLogger = Poco::Logger::get(loggerName);
         config.configure(scriptLogger, "debug", file);

         auto yScriptApi = m_scriptManager->createScriptContext(scriptLogger);
         auto stopNotifier = m_scriptManager->createStopNotifier(m_ruleStateHandler, m_ruleData->Id());

         auto scriptInterpreter = m_scriptManager->getAssociatedInterpreter(scriptProperties->interpreterName());
         m_process = scriptInterpreter->createProcess(scriptProperties->scriptPath(), loggerName, yScriptApi, stopNotifier);

      }
      catch (std::exception &ex)
      {
         YADOMS_LOG(error) << "Can not start rule " << m_ruleData->Name() << " : " << ex.what();
      }
   }

   void CRule::requestStop()
   {
      if (!!m_process)
         m_process->kill();
   }
} // namespace automation	
	
	
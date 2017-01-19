#include "stdafx.h"
#include "Rule.h"
#include "script/YScriptApiImplementation.h"
#include "script/Properties.h"
#include "script/IpcAdapter.h"
#include "script/ScriptLogConfiguration.h"

namespace automation
{
   CRule::CRule(boost::shared_ptr<const database::entities::CRule> ruleData,
                const IPathProvider& pathProvider,
                boost::shared_ptr<interpreter::IManager> interpreterManager,
                boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
                boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
                boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
                boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordAccessLayer,
                boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
                boost::shared_ptr<script::IGeneralInfo> generalInfo)
      : m_pathProvider(pathProvider),
        m_ruleData(ruleData),
        m_interpreterManager(interpreterManager)
   {
      start(pluginGateway,
            dbAcquisitionRequester,
            dbDeviceRequester,
            keywordAccessLayer,
            dbRecipientRequester,
            generalInfo);
   }

   CRule::~CRule()
   {
   }

   void CRule::start(boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
                     boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
                     boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
                     boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordAccessLayer,
                     boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
                     boost::shared_ptr<script::IGeneralInfo> generalInfo)
   {
      auto& scriptLogger = createScriptLogger(m_interpreterManager->getScriptLogFilename(m_ruleData->Id()));

      auto apiImplementation = createScriptApiImplementation(pluginGateway,
                                                             dbAcquisitionRequester,
                                                             dbDeviceRequester,
                                                             keywordAccessLayer,
                                                             dbRecipientRequester,
                                                             generalInfo,
                                                             scriptLogger);

      m_ipcAdapter = createScriptIpcAdapter(m_ruleData->Id(),
                                            apiImplementation);

      script::CProperties ruleProperties(m_ruleData,
                                         m_pathProvider);

      m_scriptInterpreter = m_interpreterManager->getInterpreterInstance(m_ruleData->Interpreter());

      m_scriptInterpreter->startScript(m_ruleData->Id(),
                                       ruleProperties.scriptPath(),
                                       m_ipcAdapter->id(),
                                       m_interpreterManager->getScriptLogFilename(m_ruleData->Id()));
   }

   Poco::Logger& CRule::createScriptLogger(const boost::filesystem::path& logFilePath) const
   {
      script::CScriptLogConfiguration config;
      auto& scriptLogger = Poco::Logger::get("script/" + std::to_string(m_ruleData->Id()));
      config.configure(scriptLogger,
                       "debug",
                       logFilePath);
      return scriptLogger;
   }

   void CRule::requestStop()
   {
      m_scriptInterpreter->stopScript(m_ruleData->Id());
   }

   boost::shared_ptr<script::IIpcAdapter> CRule::createScriptIpcAdapter(int ruleId,
                                                                        boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> apiImplementation) const
   {
      return boost::make_shared<script::CIpcAdapter>(apiImplementation,
                                                     ruleId);
   }

   boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> CRule::createScriptApiImplementation(boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
                                                                                                   boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
                                                                                                   boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
                                                                                                   boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordAccessLayer,
                                                                                                   boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
                                                                                                   boost::shared_ptr<script::IGeneralInfo> generalInfo,
                                                                                                   Poco::Logger& scriptLogger) const
   {
      return boost::make_shared<script::CYScriptApiImplementation>(scriptLogger,
                                                                   pluginGateway,
                                                                   dbAcquisitionRequester,
                                                                   dbDeviceRequester,
                                                                   keywordAccessLayer,
                                                                   dbRecipientRequester,
                                                                   generalInfo);
   }
} // namespace automation	

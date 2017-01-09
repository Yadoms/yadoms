#include "stdafx.h"
#include "Rule.h"
#include "script/StopNotifier.h"
#include "script/YScriptApiImplementation.h"
#include "script/Properties.h"
#include "script/IpcAdapter.h"

namespace automation //TODO faire une factory
{
   CRule::CRule(boost::shared_ptr<const database::entities::CRule> ruleData,
                boost::shared_ptr<interpreter::IManager> interpreterManager,
                boost::shared_ptr<IRuleStateHandler> ruleStateHandler,
                boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
                boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
                boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
                boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordAccessLayer,
                boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
                boost::shared_ptr<script::IGeneralInfo> generalInfo)
      : m_ruleData(ruleData),
        m_ruleProperties(boost::make_shared<script::CProperties>(m_ruleData)),
        m_interpreterManager(interpreterManager),
        m_ruleStateHandler(ruleStateHandler),
        m_pluginGateway(pluginGateway),
        m_dbAcquisitionRequester(dbAcquisitionRequester),
        m_dbDeviceRequester(dbDeviceRequester),
        m_keywordAccessLayer(keywordAccessLayer),
        m_dbRecipientRequester(dbRecipientRequester),
        m_generalInfo(generalInfo)
   {
      start();
   }

   CRule::~CRule()
   {
   }

   void CRule::start()
   {
      m_scriptLogger = m_interpreterManager->createScriptLogger(m_ruleData->Name(),
                                                                   m_ruleData->Id());
      m_ipcAdapter = createScriptContext(m_scriptLogger,
                                         m_ruleData->Id());

      m_scriptInterpreter = m_interpreterManager->getInterpreterInstance(m_ruleProperties->interpreterName());

      m_scriptInterpreter->startScript(m_ruleData->Id(),
                                       m_ruleProperties->scriptPath(),
                                       m_ipcAdapter->id());
   }

   void CRule::requestStop()
   {
      m_scriptInterpreter->stopScript(m_ruleData->Id());
   }

   boost::shared_ptr<script::IIpcAdapter> CRule::createScriptContext(boost::shared_ptr<shared::process::IExternalProcessLogger> scriptLogger,
                                                                     int ruleId) const
   {
      auto apiImplementation = createScriptApiImplementation(scriptLogger);

      return boost::make_shared<script::CIpcAdapter>(apiImplementation,
                                                     ruleId);
   }

   boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> CRule::createScriptApiImplementation(boost::shared_ptr<shared::process::IExternalProcessLogger> scriptLogger) const
   {
      return boost::make_shared<script::CYScriptApiImplementation>(scriptLogger,
                                                                   m_pluginGateway,
                                                                   m_dbAcquisitionRequester,
                                                                   m_dbDeviceRequester,
                                                                   m_keywordAccessLayer,
                                                                   m_dbRecipientRequester,
                                                                   m_generalInfo);
   }

   boost::shared_ptr<shared::process::IProcessObserver> CRule::createStopNotifier(boost::shared_ptr<IRuleStateHandler> ruleStateHandler,
                                                                                  int ruleId) const
   {
      //TODO cette fonction n'est normalement plus utile, la virer (ainsi que ces dépendances) après test
      return boost::make_shared<script::StopNotifier>(ruleStateHandler,
                                                      ruleId);
   }
} // namespace automation	

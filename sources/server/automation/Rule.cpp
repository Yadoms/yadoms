#include "stdafx.h"
#include "Rule.h"
#include <shared/Log.h>
#include "script/StopNotifier.h"
#include "script/YScriptApiImplementation.h"
#include "script/Properties.h"

namespace automation
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
      if (!m_scriptProcessId.empty())
      {
         YADOMS_LOG(warning) << "Can not start rule " << m_ruleData->Name() << " : already started";
         return;
      }

      auto scriptLogger = m_interpreterManager->createScriptLogger(m_ruleData->Name(),
                                                                   m_ruleData->Id());
      auto yScriptApi = createScriptContext(scriptLogger);
      auto stopNotifier = createStopNotifier(m_ruleStateHandler,
                                             m_ruleData->Id());

      m_scriptInterpreter = m_interpreterManager->getInterpreterInstance(m_ruleProperties->interpreterName());

      m_scriptProcessId = m_scriptInterpreter->startScript(m_ruleProperties->scriptPath());
   }

   void CRule::requestStop()
   {
      m_scriptInterpreter->stopScript(m_scriptProcessId);
      m_scriptProcessId.clear();
   }

   boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> CRule::createScriptContext(boost::shared_ptr<shared::process::ILogger> scriptLogger)
   {
      boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi(boost::make_shared<script::CYScriptApiImplementation>(scriptLogger,
                                                                                                                                  m_pluginGateway,
                                                                                                                                  m_dbAcquisitionRequester,
                                                                                                                                  m_dbDeviceRequester,
                                                                                                                                  m_keywordAccessLayer,
                                                                                                                                  m_dbRecipientRequester,
                                                                                                                                  m_generalInfo));
      return yScriptApi;
   }

   boost::shared_ptr<shared::process::IProcessObserver> CRule::createStopNotifier(boost::shared_ptr<IRuleStateHandler> ruleStateHandler,
                                                                                  int ruleId) const
   {
      return boost::make_shared<script::StopNotifier>(ruleStateHandler,
                                                      ruleId);
   }
} // namespace automation	



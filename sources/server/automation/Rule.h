#pragma once
#include "IRule.h"
#include "database/entities/Entities.h"
#include "IRuleStateHandler.h"
#include <shared/process/IProcess.h>
#include "interpreter/IManager.h"
#include <shared/process/IProcessObserver.h>
#include <shared/script/yScriptApi/IYScriptApi.h>
#include "script/IGeneralInfo.h"
#include "dataAccessLayer/IKeywordManager.h"
#include "database/IRecipientRequester.h"
#include "database/IAcquisitionRequester.h"
#include "database/IDeviceRequester.h"
#include "communication/ISendMessageAsync.h"
#include "script/IProperties.h"

namespace automation
{
   //-----------------------------------------------------
   ///\brief A rule
   //-----------------------------------------------------
   class CRule : public IRule
   {
   public:
      CRule(boost::shared_ptr<const database::entities::CRule> ruleData,
            boost::shared_ptr<interpreter::IManager> interpreterManager,
            boost::shared_ptr<IRuleStateHandler> ruleStateHandler,
            boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
            boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
            boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
            boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordAccessLayer,
            boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
            boost::shared_ptr<script::IGeneralInfo> generalInfo);
      virtual ~CRule();

      // IRule Implementation
      void requestStop() override;
      // [END] IRule Implementation

   protected:
      //-----------------------------------------------------
      ///\brief               Start the rule
      //-----------------------------------------------------
      void start();

      //-----------------------------------------------------
      ///\brief               Create the script context (IYScriptApi implementation)
      ///\param[in] scriptLogger The logger used for rule
      ///\return              A script context instance
      //-----------------------------------------------------
      boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> createScriptContext(boost::shared_ptr<shared::process::ILogger> scriptLogger);

      //-----------------------------------------------------
      ///\brief               Create the stop notifier
      ///\param[in] ruleStateHandler   The global state handler
      ///\param[in] ruleId    The rule ID
      ///\return              A stop notifier instance
      //-----------------------------------------------------
      boost::shared_ptr<shared::process::IProcessObserver> createStopNotifier(boost::shared_ptr<IRuleStateHandler> ruleStateHandler,
                                                                              int ruleId) const;

   private:
      //TODO faire le ménage dans les données
      boost::shared_ptr<const database::entities::CRule> m_ruleData;
      boost::shared_ptr<const script::IProperties> m_ruleProperties;
      boost::shared_ptr<interpreter::IManager> m_interpreterManager;
      boost::shared_ptr<IRuleStateHandler> m_ruleStateHandler;

      boost::shared_ptr<communication::ISendMessageAsync> m_pluginGateway;
      boost::shared_ptr<database::IAcquisitionRequester> m_dbAcquisitionRequester;
      boost::shared_ptr<database::IDeviceRequester> m_dbDeviceRequester;
      boost::shared_ptr<dataAccessLayer::IKeywordManager> m_keywordAccessLayer;
      boost::shared_ptr<database::IRecipientRequester> m_dbRecipientRequester;
      boost::shared_ptr<script::IGeneralInfo> m_generalInfo;

      boost::shared_ptr<interpreter::IInstance> m_scriptInterpreter;
      std::string m_scriptProcessId;
   };
} // namespace automation	



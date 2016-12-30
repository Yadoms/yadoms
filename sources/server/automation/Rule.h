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
      //-----------------------------------------------------
      ///\brief               Minimum rule duration
      //-----------------------------------------------------
      static const boost::chrono::milliseconds m_MinRuleDuration;

      //-----------------------------------------------------
      ///\brief               Rule data
      //-----------------------------------------------------
      boost::shared_ptr<const database::entities::CRule> m_ruleData;

      //-----------------------------------------------------
      ///\brief               The script manager
      //-----------------------------------------------------
      boost::shared_ptr<interpreter::IManager> m_interpreterManager;

      //-----------------------------------------------------
      ///\brief               The script process
      //-----------------------------------------------------
      boost::shared_ptr<shared::process::IProcess> m_process;

      //-----------------------------------------------------
      ///\brief               The rule state handler
      //-----------------------------------------------------
      boost::shared_ptr<IRuleStateHandler> m_ruleStateHandler;

      boost::shared_ptr<communication::ISendMessageAsync> m_pluginGateway;
      boost::shared_ptr<database::IAcquisitionRequester> m_dbAcquisitionRequester;
      boost::shared_ptr<database::IDeviceRequester> m_dbDeviceRequester;
      boost::shared_ptr<dataAccessLayer::IKeywordManager> m_keywordAccessLayer;
      boost::shared_ptr<database::IRecipientRequester> m_dbRecipientRequester;
      boost::shared_ptr<script::IGeneralInfo> m_generalInfo;
   };
} // namespace automation	



#pragma once
#include "IRule.h"
#include "database/entities/Entities.h"
#include "interpreter/IManager.h"
#include <shared/script/yScriptApi/IYScriptApi.h>
#include "script/IGeneralInfo.h"
#include "dataAccessLayer/IKeywordManager.h"
#include "database/IRecipientRequester.h"
#include "database/IAcquisitionRequester.h"
#include "database/IDeviceRequester.h"
#include "communication/ISendMessageAsync.h"
#include "script/IIpcAdapter.h"

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
      void start(boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
                 boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
                 boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
                 boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordAccessLayer,
                 boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
                 boost::shared_ptr<script::IGeneralInfo> generalInfo);

      boost::shared_ptr<script::IIpcAdapter> createScriptIpcAdapter(int ruleId,
                                                                    boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> apiImplementation) const;

      boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> createScriptApiImplementation(boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
                                                                                               boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
                                                                                               boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
                                                                                               boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordAccessLayer,
                                                                                               boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
                                                                                               boost::shared_ptr<script::IGeneralInfo> generalInfo) const;

   private:
      boost::shared_ptr<const database::entities::CRule> m_ruleData;
      boost::shared_ptr<interpreter::IManager> m_interpreterManager;

      boost::shared_ptr<interpreter::IInstance> m_scriptInterpreter;
      boost::shared_ptr<script::IIpcAdapter> m_ipcAdapter;
   };
} // namespace automation	

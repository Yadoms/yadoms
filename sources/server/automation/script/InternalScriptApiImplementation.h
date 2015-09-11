#pragma once
#include "YScriptApiImplementation.h"
#include "IInternalScriptApiImplementation.h"

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The internal script API implementation interface
   //-----------------------------------------------------
   class InternalScriptApiImplementation : public IInternalScriptApiImplementation
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] ruleLogger Logger to use for script logging
      ///\param[in] interpreterErrorRaiser Interpretor error signaler
      ///\param[in] pluginGateway Plugin access to do actions on plugins
      ///\param[in] configurationManager  Configuration manager (to gain access to Yadoms configuration from rules scripts)
      ///\param[in] acquisitionNotifier Acquisition notifier, used to be notified on new acquisitions on keywords
      ///\param[in] dbAcquisitionRequester  Database acquisition requester
      ///\param[in] dbDeviceRequester  Database device requester
      ///\param[in] dbKeywordRequester  Database keyword requester
      ///\param[in] dbRecipientRequester  Database recipient requester
      ///\param[in] generalInfo  Database acquisition requester
      //-----------------------------------------------------
      InternalScriptApiImplementation(boost::shared_ptr<shared::script::ILogger> ruleLogger,
         boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
         boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
         boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
         boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
         boost::shared_ptr<database::IKeywordRequester> dbKeywordRequester,
         boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
         boost::shared_ptr<IGeneralInfo> generalInfo);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~InternalScriptApiImplementation();

   protected:
      // IInternalScriptApiImplementation Implementation
      virtual shared::script::yScriptApi::IYScriptApi& scriptApi();
      virtual bool ruleEnabled() const;
      // [END] IInternalScriptApiImplementation Implementation

   private:
      //-----------------------------------------------------
      ///\brief               The script Api implementation
      //-----------------------------------------------------
      CYScriptApiImplementation m_scriptApi;
   };

} } // namespace automation::script


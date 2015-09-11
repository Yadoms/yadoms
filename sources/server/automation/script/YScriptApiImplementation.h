#pragma once
#include <shared/script/yScriptApi/IYScriptApi.h>
#include <shared/script/ILogger.h>
#include "database/IAcquisitionRequester.h"
#include "database/IDeviceRequester.h"
#include "database/IKeywordRequester.h"
#include "database/IRecipientRequester.h"
#include "dataAccessLayer/IConfigurationManager.h"
#include "communication/ISendMessageAsync.h"
#include "IGeneralInfo.h"

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The Yadoms script API implementation
   //-----------------------------------------------------
   class CYScriptApiImplementation : public shared::script::yScriptApi::IYScriptApi
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
      CYScriptApiImplementation(boost::shared_ptr<shared::script::ILogger> ruleLogger,
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
      virtual ~CYScriptApiImplementation();

      // shared::script::yScriptApi::IYScriptApi implementation
      virtual int getKeywordId(const std::string& deviceName, const std::string& keywordName) const;
      virtual std::string readKeyword(int keywordId) const;
      virtual std::string waitForAcquisition(int keywordId, const std::string& timeout = std::string()) const;
      virtual std::pair<int, std::string> waitForAcquisitions(const std::vector<int> keywordIdList, const std::string& timeout) const;
      virtual void writeKeyword(int keywordId, const std::string& newState);
      virtual void sendNotification(int keywordId, int recipientId, const std::string& message);
      virtual std::string getInfo(EInfoKeys key) const;
      virtual void ruleEnable(bool enable = true);
      // [END] shared::script::yScriptApi::IYScriptApi implementation

   public: // Public methods, for Yadoms internal usage
      //-----------------------------------------------------
      ///\brief               Check if rule is still enabled
      //-----------------------------------------------------
      bool ruleEnabled() const;

      //-----------------------------------------------------
      ///\brief               Check if the provided keyword ID exists
      ///\param[in] keywordId Keyword ID to check
      ///\throw std::out_of_range if keyword not found
      //-----------------------------------------------------
      void assertExistingKeyword(int keywordId) const;

      //-----------------------------------------------------
      ///\brief               Check if the provided recipient ID exists
      ///\param[in] recipientId recipient ID to check
      ///\throw std::out_of_range if recipient not found
      //-----------------------------------------------------
      void assertExistingRecipient(int recipientId) const;

   private:
      //-----------------------------------------------------
      ///\brief               The rule logger
      //-----------------------------------------------------
      boost::shared_ptr<shared::script::ILogger> m_ruleLogger;

      //-----------------------------------------------------
      ///\brief               The plugin access (to send commands to plugins)
      //-----------------------------------------------------
      boost::shared_ptr<communication::ISendMessageAsync> m_pluginGateway;

      //-----------------------------------------------------
      ///\brief               Database acquisition requester
      //-----------------------------------------------------
      boost::shared_ptr<database::IAcquisitionRequester> m_dbAcquisitionRequester;

      //-----------------------------------------------------
      ///\brief               Database device requester
      //-----------------------------------------------------
      boost::shared_ptr<database::IDeviceRequester> m_dbDeviceRequester;

      //-----------------------------------------------------
      ///\brief               Database keyword requester
      //-----------------------------------------------------
      boost::shared_ptr<database::IKeywordRequester> m_dbKeywordRequester;

      //-----------------------------------------------------
      ///\brief               Database recipient requester
      //-----------------------------------------------------
      boost::shared_ptr<database::IRecipientRequester> m_dbRecipientRequester;

      //-----------------------------------------------------
      ///\brief               General information requester
      //-----------------------------------------------------
      boost::shared_ptr<IGeneralInfo> m_generalInfo;

      //-----------------------------------------------------
      ///\brief               Flag set if rule is still enabled (true by default)
      //-----------------------------------------------------
      bool m_ruleEnabled;
   };

} } // namespace automation::script
	
	
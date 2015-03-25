#pragma once
#include <shared/script/yScriptApi/IYScriptApi.h>
#include "../../database/IAcquisitionRequester.h"
#include "../../dataAccessLayer/IConfigurationManager.h"
#include "../../communication/ISendMessageAsync.h"
#include <shared/notification/NotificationCenter.h>
#include "IGeneralInfo.h"
#include "ILogger.h"

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
      ///\param[in] pluginGateway Plugin access to do actions on plugins
      ///\param[in] configurationManager  Configuration manager (to gain access to Yadoms configuration from rules scripts)
      ///\param[in] notificationCenter Notification center, used to get notified on keyword state changes
      ///\param[in] dbAcquisitionRequester  Database acquisition requester
      ///\param[in] generalInfo  Database acquisition requester
      //-----------------------------------------------------
      CYScriptApiImplementation(boost::shared_ptr<ILogger> ruleLogger,
         boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
         boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
         boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter,//TODO utile ?
         boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
         boost::shared_ptr<IGeneralInfo> generalInfo);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CYScriptApiImplementation();

      // shared::script::yScriptApi::IYScriptApi implementation
      virtual std::string readKeyword(int keywordId) const;
      virtual std::string waitForEvent(int keywordId, const std::string& timeout = std::string()) const;
      virtual std::pair<int, std::string> waitForEvents(std::vector<int> keywordIdList, const std::string& timeout) const;
      virtual void writeKeyword(int keywordId, const std::string& newState);
      virtual void sendNotification(int keywordId, int recipientId, const std::string& message);
      virtual std::string getInfo(const std::string& key) const;
      virtual void log(const std::string& message);
      virtual void logError(const std::string& message);
      virtual void fail(const std::string& errorMessage);
      // [END] shared::script::yScriptApi::IYScriptApi implementation

   private:
      //-----------------------------------------------------
      ///\brief               The rule logger
      //-----------------------------------------------------
      boost::shared_ptr<ILogger> m_ruleLogger;

      //-----------------------------------------------------
      ///\brief               The plugin access (to send commands to plugins)
      //-----------------------------------------------------
      boost::shared_ptr<communication::ISendMessageAsync> m_pluginGateway;

      //-----------------------------------------------------
      ///\brief               The notification center
      //-----------------------------------------------------
      boost::shared_ptr<shared::notification::CNotificationCenter> m_notificationCenter;

      //-----------------------------------------------------
      ///\brief               Database acquisition requester
      //-----------------------------------------------------
      boost::shared_ptr<database::IAcquisitionRequester> m_dbAcquisitionRequester;

      //-----------------------------------------------------
      ///\brief               General information requester
      //-----------------------------------------------------
      boost::shared_ptr<IGeneralInfo> m_generalInfo;
   };

} } // namespace automation::script
	
	
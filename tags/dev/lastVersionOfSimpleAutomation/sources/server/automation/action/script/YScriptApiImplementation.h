#pragma once
#include <shared/script/yScriptApi/IYScriptApi.h>
#include "../../../database/IAcquisitionRequester.h"
#include "../../../communication/ISendMessageAsync.h"
#include <shared/notification/NotificationCenter.h>

namespace automation { namespace action { namespace script
{
   //-----------------------------------------------------
   ///\brief The Yadoms script API implementation
   //-----------------------------------------------------
   class CYScriptApiImplementation : public shared::script::yScriptApi::IYScriptApi
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] pluginGateway Plugin access to do actions on plugins
      ///\param[in] notificationCenter Notification center, used to get notified on keyword state changes
      ///\param[in] dbAcquisitionRequester  Database acquisition requester
      //-----------------------------------------------------
      CYScriptApiImplementation(boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
         boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter,
         boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester);

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
      // [END] shared::script::yScriptApi::IYScriptApi implementation

   private:
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
   };

} } } // namespace automation::action::script
	
	
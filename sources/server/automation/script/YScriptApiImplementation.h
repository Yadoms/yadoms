#pragma once
#include <shared/script/yScriptApi/IYScriptApi.h>
#include "database/IAcquisitionRequester.h"
#include "database/IDeviceRequester.h"
#include "database/IRecipientRequester.h"
#include "communication/ISendMessageAsync.h"
#include "IGeneralInfo.h"
#include "notification/action/WaitAction.hpp"
#include "notification/acquisition/Notification.hpp"
#include <server/dataAccessLayer/IKeywordManager.h>
#include <Poco/Logger.h>

namespace automation
{
   namespace script
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
         ///\param[in] dbAcquisitionRequester  Database acquisition requester
         ///\param[in] dbDeviceRequester  Database device requester
         ///\param[in] keywordAccessLayer  Database keyword access layer
         ///\param[in] dbRecipientRequester  Database recipient requester
         ///\param[in] generalInfo  Database acquisition requester
         //-----------------------------------------------------
         CYScriptApiImplementation(Poco::Logger& ruleLogger,
                                   boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
                                   boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
                                   boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
                                   boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordAccessLayer,
                                   boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
                                   boost::shared_ptr<IGeneralInfo> generalInfo);

         //-----------------------------------------------------
         ///\brief               Destructor
         //-----------------------------------------------------
         virtual ~CYScriptApiImplementation();

         // shared::script::yScriptApi::IYScriptApi implementation
         int getKeywordId(const std::string& deviceName,
                          const std::string& keywordName) const override;
         int getRecipientId(const std::string& firstName,
                            const std::string& lastName) const override;
         std::string readKeyword(int keywordId) const override;
         std::string waitForNextAcquisition(int keywordId,
                                            const std::string& timeout = std::string()) const override;
         std::pair<int, std::string> waitForNextAcquisitions(const std::vector<int>& keywordIdList,
                                                             const std::string& timeout) const override;
         shared::script::yScriptApi::CWaitForEventResult waitForEvent(const std::vector<int>& keywordIdList,
                                                                      bool receiveDateTimeEvent,
                                                                      const std::string& timeout) const override;
         std::vector<int> getKeywordsByCapacity(const std::string& capacity) const override;
         void writeKeyword(int keywordId,
                           const std::string& newState) override;
         void sendNotification(int keywordId,
                               int recipientId, const std::string& message) override;
         std::string getInfo(EInfoKeys key) const override;
         std::string getKeywordName(int keywordId) const override;
         std::string getKeywordDeviceName(int keywordId) const override;
         // [END] shared::script::yScriptApi::IYScriptApi implementation

         //
         // Public methods, for Yadoms internal usage
         //

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

      protected:
         //-----------------------------------------------------
         ///\brief               Wait for an action (used in acquisition notification)
         ///\param[in] action    Action object to wait on
         ///\param[in] timeout   Timeout, as string. Can be a duration (format \"hh:mm:ss.xxx\") or a dateTime (format \"YYYY-MM-DD hh:mm:ss.xxx\"). No timeout if empty.
         ///\return              The acquisition (null if timeout)
         //-----------------------------------------------------
         static boost::shared_ptr<notification::acquisition::CNotification> waitForAction(boost::shared_ptr<notification::action::CWaitAction<notification::acquisition::CNotification >> action,
                                                                                          const std::string& timeout);

      private:
         //-----------------------------------------------------
         ///\brief               The rule logger
         //-----------------------------------------------------
         Poco::Logger& m_ruleLogger;

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
         boost::shared_ptr<dataAccessLayer::IKeywordManager> m_keywordAccessLayer;

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
   }
} // namespace automation::script



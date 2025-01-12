#pragma once
#include <curlpp/Easy.hpp>
#include <plugin_cpp_api/IPlugin.h>
#include "IMSConfiguration.h"
#include "PendingMail.h"

namespace yApi = shared::plugin::yPluginApi;

/// @brief	This class is the MailSender plugin
/// @note   This plugin send a mail when receive a notification
class CMailSender final : public plugin_cpp_api::IPlugin
{
public:
   enum class ESendResult
   {
      kOk,
      kTemporaryError,
      kFatalError
   };

   CMailSender();
   CMailSender(const CMailSender&) = delete;
   CMailSender(const CMailSender&&) = delete;
   CMailSender operator=(const CMailSender&) = delete;
   CMailSender operator=(const CMailSender&&) = delete;
   ~CMailSender() override = default;

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

private:
   void onSendMailRequest(boost::shared_ptr<yApi::IYPluginApi> api,
                          const std::string& sendMailRequest);

   void sendPendingMails(boost::shared_ptr<yApi::IYPluginApi> api);

   ESendResult sendMail(boost::shared_ptr<yApi::IYPluginApi> api,
                        const boost::shared_ptr<CPendingMail> mail) const;

   /// @brief Declare the device and all keywords associated
   /// @param[in] api                  pointer to the API
   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const;

   /// @brief Update the configuration of the plugin after a change
   /// @param[in] api                   pointer to the API
   /// @param[in] newConfigurationData  The new configuration of the module
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                              const boost::shared_ptr<shared::CDataContainer>& newConfigurationData) const;

   /// @brief Set security to request
   /// @param mailSendRequest request to which set security
   void setSecurity(curlpp::Easy& mailSendRequest) const;

   /// @brief Set authentication to SMTP server
   /// @param mailSendRequest request to which set authentication
   void setAuthentication(curlpp::Easy& mailSendRequest) const;

   /// @brief Format the email subject
   /// @param text The text to send (first line can be used to build subject)
   /// @return The formatted subject
   std::string formatSubject(const std::string& text) const;

   static bool isTemporaryError(CURLcode curlCode);

   /// @brief The device name
   std::string m_deviceName;

   /// @briefThe plugin configuration
   boost::shared_ptr<IMSConfiguration> m_configuration;


   /// @brief Message historization object
   boost::shared_ptr<yApi::historization::CMessage> m_messageKeyword;

   boost::shared_ptr<shared::event::CEventTimer> m_retryTimer;
   std::queue<boost::shared_ptr<CPendingMail>> m_pendingMailsQueue;
};

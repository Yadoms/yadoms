#include "stdafx.h"
#include "MailSender.h"

#include <regex>
#include <curlpp/Infos.hpp>
#include <shared/plugin/yPluginApi/historization/Message.h>
#include <shared/exception/EmptyResult.hpp>
#include <plugin_cpp_api/ImplementationHelper.h>
#include "MSConfiguration.h"
#include <shared/Log.h>
#include <shared/http/curlppHelpers.h>
#include <curlpp/Options.hpp>

#include "MailBodyBuilder.h"

IMPLEMENT_PLUGIN(CMailSender)

/// @brief Recipient field used to retrieve e-mail from a recipient
static const std::string MailRecipientField("email");

CMailSender::CMailSender() :
   m_deviceName("MailSender"),
   m_configuration(boost::make_shared<CMSConfiguration>()),
   m_messageKeyword(boost::make_shared<yApi::historization::CMessage>("message", MailRecipientField, yApi::EKeywordAccessMode::kGetSet))
{
}

void CMailSender::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "MailSender is starting...";


   m_configuration->initializeWith(api->getConfiguration());
   // Declaration of the device and Associated keywords
   declareDevice(api);

   enum { kRetryTimer = yApi::IYPluginApi::kPluginFirstEventId };
   m_retryTimer = api->getEventHandler().createTimer(kRetryTimer,
                                                     shared::event::CEventTimer::kOneShot);

   // the main loop
   YADOMS_LOG(information) << "MailSender plugin is running...";
   api->setPluginState(yApi::historization::EPluginState::kRunning);

   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            YADOMS_LOG(information) << "Stop requested";
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            return;
         }

      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
            onUpdateConfiguration(api, api->getEventHandler().getEventData<boost::shared_ptr<shared::CDataContainer>>());
            m_retryTimer->stop();
            m_pendingMailsQueue = std::queue<boost::shared_ptr<CPendingMail>>();
            api->setPluginState(yApi::historization::EPluginState::kRunning);
            break;
         }

      case yApi::IYPluginApi::kEventDeviceCommand:
         {
            // Command received
            const auto command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>();
            YADOMS_LOG(information) << "Command received :" << yApi::IDeviceCommand::toString(command);

            try
            {
               if (!boost::iequals(command->getKeyword(), m_messageKeyword->getKeyword()))
               {
                  YADOMS_LOG(information) << "Received command for unknown keyword from Yadoms : " << yApi::IDeviceCommand::toString(command);
                  break;
               }

               onSendMailRequest(api, command->getBody());
            }
            catch (std::exception& e)
            {
               YADOMS_LOG(error) << "Fail to send command " << yApi::IDeviceCommand::toString(command) << ", error : " << e.what();
            }
         }
         break;

      case kRetryTimer:
         {
            sendPendingMails(api);
            break;
         }

      default:
         {
            YADOMS_LOG(warning) << "Unknown message id " << api->getEventHandler().getEventId();
            break;
         }
      }
   }
}

void CMailSender::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (api->deviceExists(m_deviceName))
      return;

   api->declareDevice(m_deviceName, m_deviceName, m_deviceName, m_messageKeyword);
}

void CMailSender::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                        const boost::shared_ptr<shared::CDataContainer>& newConfigurationData) const
{
   // Configuration was updated
   YADOMS_LOG(information) << "Configuration was updated";
   BOOST_ASSERT(!newConfigurationData->empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration->initializeWith(newConfigurationData);
}

void CMailSender::onSendMailRequest(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const std::string& sendMailRequest)
{
   m_messageKeyword->setCommand(sendMailRequest);

   m_pendingMailsQueue.push(boost::make_shared<CPendingMail>(m_messageKeyword->to(),
                                                             m_messageKeyword->body()));

   m_retryTimer->stop();

   sendPendingMails(api);
}

void CMailSender::sendPendingMails(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(debug) << "Send pending emails (" << m_pendingMailsQueue.size() << ")";

   while (!m_pendingMailsQueue.empty())
   {
      const auto mail = m_pendingMailsQueue.front();
      switch (sendMail(api, mail))
      {
      case ESendResult::kOk:
         m_pendingMailsQueue.pop();
         break;

      case ESendResult::kTemporaryError:
         if (!mail->retry())
         {
            YADOMS_LOG(warning) << "Email has permanent errors, skip it";
            m_pendingMailsQueue.pop();
         }
         else
         {
            YADOMS_LOG(warning) << "Unable to send email, will retry later...";
            m_retryTimer->start(boost::posix_time::minutes(2));
            return;
         }
         break;

      case ESendResult::kFatalError:
         YADOMS_LOG(error) << "Fatal error sending email, skip it";
         m_pendingMailsQueue.pop();
         break;
      }
   }
}

CMailSender::ESendResult CMailSender::sendMail(boost::shared_ptr<yApi::IYPluginApi> api,
                                               const boost::shared_ptr<CPendingMail> mail) const
{
   YADOMS_LOG(debug) << "Sending email...";

   try
   {
      const auto from = m_configuration->getSenderEmail();
      const auto to = api->getRecipientValue(mail->recipientId(), MailRecipientField);

      curlpp::Easy mailSendRequest;
      mailSendRequest.setOpt(new curlpp::options::Url(m_configuration->getHost()));
      mailSendRequest.setOpt(new curlpp::options::Port(m_configuration->getPort()));
      mailSendRequest.setOpt(new curlpp::options::MailFrom(from));
      mailSendRequest.setOpt(new curlpp::options::MailRcpt({to}));

      const auto bodyString = CMailBodyBuilder(from,
                                               {to})
                              .setSubject(formatSubject(mail->body()))
                              .setAsciiBody(mail->body())
                              .build();

      size_t bytesRead = 0;
      mailSendRequest.setOpt(new curlpp::options::ReadFunction([&bodyString, &bytesRead](char* ptr, size_t size, size_t nbItems)
      {
         if (size == 0 || nbItems == 0 || size * nbItems < 1 || bytesRead > bodyString.size())
            return static_cast<size_t>(0);

         const size_t len = std::min(bodyString.size() - bytesRead, size * nbItems);
         memcpy(ptr, &bodyString.c_str()[bytesRead], len);
         bytesRead += len;

         return len;
      }));
      mailSendRequest.setOpt(new curlpp::options::Upload(true));

      setSecurity(mailSendRequest);
      setAuthentication(mailSendRequest);

      shared::http::CCurlppHelpers::setProxy(mailSendRequest,
                                             {});

      try
      {
         mailSendRequest.perform();
         YADOMS_LOG(information) << "Email successfully sent to " << to;
         return ESendResult::kOk;
      }
      catch (const curlpp::LibcurlRuntimeError& error)
      {
         YADOMS_LOG(warning) << "Fail to send email : " << error.what() << ", code " << error.whatCode();

         return isTemporaryError(error.whatCode()) ? ESendResult::kTemporaryError : ESendResult::kFatalError;
      }
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Invalid Mail sending request : " << e.what();
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "Error sending email : " << e.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Error sending Mail";
   }

   return ESendResult::kFatalError;
}


void CMailSender::setSecurity(curlpp::Easy& mailSendRequest) const
{
   switch (m_configuration->getSecurityMode())
   {
   case ESecurityMode::kNoneValue:
      mailSendRequest.setOpt(new curlpp::options::UseSsl(CURLUSESSL_NONE));
      return;
   case ESecurityMode::kSSLValue:
      mailSendRequest.setOpt(new curlpp::options::UseSsl(CURLUSESSL_TRY));
      mailSendRequest.setOpt(new curlpp::options::SslVerifyHost(2));
#ifdef WIN32
      mailSendRequest.setOpt(curlpp::options::SslOptions(CURLSSLOPT_NATIVE_CA));
#endif
      return;
   case ESecurityMode::kTLSValue:
      mailSendRequest.setOpt(new curlpp::options::UseSsl(CURLUSESSL_ALL));
      mailSendRequest.setOpt(new curlpp::options::SslVerifyHost(2));
#ifdef WIN32
      mailSendRequest.setOpt(curlpp::options::SslOptions(CURLSSLOPT_NATIVE_CA));
#endif
      return;
   case ESecurityMode::kAutomaticValue:
      mailSendRequest.setOpt(new curlpp::options::SslVerifyHost(2));
#ifdef WIN32
      mailSendRequest.setOpt(curlpp::options::SslOptions(CURLSSLOPT_NATIVE_CA));
#endif
      switch (m_configuration->getPort())
      {
      case 465: //default ssl port
         mailSendRequest.setOpt(new curlpp::options::UseSsl(CURLUSESSL_TRY));
         return;
      case 587: //default tsl port
         mailSendRequest.setOpt(new curlpp::options::UseSsl(CURLUSESSL_ALL));
         return;
      default: //25, 26 or other
         mailSendRequest.setOpt(new curlpp::options::UseSsl(CURLUSESSL_TRY));
         return;
      }
   default:
      throw std::invalid_argument("Unknown security mode " + std::to_string(m_configuration->getSecurityMode()));
   }
}

void CMailSender::setAuthentication(curlpp::Easy& mailSendRequest) const
{
   if (!m_configuration->getAuthenticationRequired())
      return;

   mailSendRequest.setOpt(curlpp::options::UserPwd(m_configuration->getLogin() + ":" + m_configuration->getPassword()));
}

std::string CMailSender::formatSubject(const std::string& text) const
{
   const auto firstTextLine = text.substr(0, text.find_first_of("\r\n"));
   return std::regex_replace(m_configuration->getMailSubject(),
                             std::regex("{m}", std::regex_constants::ECMAScript | std::regex_constants::extended),
                             firstTextLine);
}

bool CMailSender::isTemporaryError(CURLcode curlCode)
{
   switch (curlCode) // NOLINT(clang-diagnostic-switch-enum)
   {
   case CURLE_COULDNT_RESOLVE_PROXY:
   case CURLE_COULDNT_RESOLVE_HOST:
   case CURLE_COULDNT_CONNECT:
   case CURLE_WEIRD_SERVER_REPLY:
   case CURLE_REMOTE_ACCESS_DENIED:
   case CURLE_OPERATION_TIMEDOUT:
   case CURLE_SEND_ERROR:
   case CURLE_RECV_ERROR:
   case CURLE_AGAIN:
   case CURLE_NO_CONNECTION_AVAILABLE:
      return true;
   default:
      return false;
   }
}

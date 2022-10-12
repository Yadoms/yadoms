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
               if (boost::iequals(command->getKeyword(), m_messageKeyword->getKeyword()))
                  onSendMailRequest(api, command->getBody());
               else
                  YADOMS_LOG(information) << "Received command for unknown keyword from Yadoms : " << yApi::IDeviceCommand::toString(command);
            }
            catch (std::exception& e)
            {
               YADOMS_LOG(error) << "Fail to send command " << yApi::IDeviceCommand::toString(command) << ", error : " << e.what();
            }
         }
         break;

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
   YADOMS_LOG(information) << "Configuration was updated...";
   BOOST_ASSERT(!newConfigurationData->empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration->initializeWith(newConfigurationData);
}

void CMailSender::onSendMailRequest(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const std::string& sendMailRequest) const
{
   try
   {
      m_messageKeyword->setCommand(sendMailRequest);

      const auto from = m_configuration->getSenderEmail();
      const auto to = api->getRecipientValue(m_messageKeyword->to(), MailRecipientField);

      curlpp::Easy mailSendRequest;
      mailSendRequest.setOpt(new curlpp::options::Url(m_configuration->getHost()));
      mailSendRequest.setOpt(new curlpp::options::Port(m_configuration->getPort()));
      mailSendRequest.setOpt(new curlpp::options::MailFrom(from));
      mailSendRequest.setOpt(new curlpp::options::MailRcpt({to}));

      const auto bodyString = CMailBodyBuilder(from,
                                               {to})
                              .setSubject(formatSubject(m_messageKeyword->body()))
                              .setAsciiBody(m_messageKeyword->body())
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

      try
      {
         mailSendRequest.perform();
      }
      catch (const curlpp::LibcurlRuntimeError& error)
      {
         const auto message = (boost::format("Fail to send email : %1%, code %2%") % error.what() % error.whatCode()).str();
         throw std::runtime_error(message);
      }

      YADOMS_LOG(information) << "Email successfully sent to " << to;

      //TODO gérer le proxy
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Invalid Mail sending request \"" << sendMailRequest << "\" : " << e.what();
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "Error sending Mail : " << e.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Error sending Mail";
   }
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

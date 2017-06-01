#include "stdafx.h"
#include "MailSender.h"
#include <Poco/Net/MailMessage.h>
#include <shared/plugin/yPluginApi/historization/Message.h>
#include <shared/exception/EmptyResult.hpp>
#include <plugin_cpp_api/ImplementationHelper.h>
#include "MSConfiguration.h"
#include "SmtpServiceProviderFactory.h"
#include <shared/Log.h>
#include "CertificatePassphraseProvider.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CMailSender)

CMailSender::CMailSender() :
   m_deviceName("MailSender"),
   m_configuration(boost::make_shared<CMSConfiguration>()),
   m_certificatePassphraseProvider(new CCertificatePassphraseProvider(m_configuration)),
   m_mailId("email"),
   m_messageKeyword(boost::make_shared<yApi::historization::CMessage>("message", m_mailId, yApi::EKeywordAccessMode::kGetSet))
{
}

CMailSender::~CMailSender()
{
}

void CMailSender::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "MailSender is starting..." ;


   m_configuration->initializeWith(api->getConfiguration());
   // Declaration of the device and Associated keywords
   declareDevice(api);

   // the main loop
   YADOMS_LOG(information) << "MailSender plugin is running..." ;

   while (1)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            YADOMS_LOG(information) << "Stop requested" ;
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            return;
         }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());
            break;
         }

      case yApi::IYPluginApi::kEventDeviceCommand:
         {
            // Command received
            auto command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>();
            YADOMS_LOG(information) << "Command received :" << yApi::IDeviceCommand::toString(command) ;

            try
            {
               if (boost::iequals(command->getKeyword(), m_messageKeyword->getKeyword()))
                  onSendMailRequest(api, command->getBody());
               else
               YADOMS_LOG(information) << "Received command for unknown keyword from Yadoms : " << yApi::IDeviceCommand::toString(command) ;
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
                                        const shared::CDataContainer& newConfigurationData) const
{
   // Configuration was updated
   YADOMS_LOG(information) << "Configuration was updated..." ;
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration->initializeWith(newConfigurationData);
}

void CMailSender::onSendMailRequest(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const std::string& sendMailRequest)
{
   try
   {
      m_messageKeyword->setCommand(sendMailRequest);

      std::string from = m_configuration->getSenderEmail();
      std::string to = getRecipientMail(api, m_messageKeyword->to());

      std::string subject = "Yadoms Notification !";
      Poco::Net::MailMessage message;
      message.setSender(from);
      message.addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT, to));
      message.setSubject(subject);
      message.setContentType("text/plain; charset=UTF-8");
      message.setContent(m_messageKeyword->body(), Poco::Net::MailMessage::ENCODING_8BIT);

      boost::shared_ptr<ISmtpServiceProvider> mailServiceProvider = CSmtpServiceProviderFactory::CreateSmtpServer(m_configuration, m_certificatePassphraseProvider);
      if (mailServiceProvider->sendMail(message))
      {
         YADOMS_LOG(information) << "Email successfully sent to " << to ;
      }
      else
      {
         YADOMS_LOG(error) << "Fail to send email to " << to ;
      }
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Invalid Mail sending request \"" << sendMailRequest << "\" : " << e.what() ;
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "Error sending Mail : " << e.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Error sending Mail" ;
   }
}


std::string CMailSender::getRecipientMail(boost::shared_ptr<yApi::IYPluginApi> api,
                                          int recipientId) const
{
   try
   {
      return api->getRecipientValue(recipientId, m_mailId);
   }
   catch (shared::exception::CEmptyResult& e)
   {
      throw shared::exception::CInvalidParameter(std::string("Recipient not found, or e-mail not found for recipient : ") + e.what());
   }
}


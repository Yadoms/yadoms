#include "stdafx.h"
#include "MailSender.h"
#include <Poco/Net/MailMessage.h>
#include <shared/plugin/yPluginApi/historization/Message.h>
#include <shared/exception/EmptyResult.hpp>
#include <plugin_cpp_api/ImplementationHelper.h>
#include "MSConfiguration.h"
#include "SmtpServiceProviderFactory.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CMailSender)

CMailSender::CMailSender() :
   m_deviceName("MailSender"),
   m_configuration(new CMSConfiguration()),
   m_mailId("email"),
   m_messageKeyword(boost::make_shared<yApi::historization::CMessage>("message",
                                                                      m_mailId,
                                                                      yApi::EKeywordAccessMode::kGetSet))
{
}

CMailSender::~CMailSender()
{
}

void CMailSender::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      std::cout << "MailSender is starting..." << std::endl;


      m_configuration->initializeWith(api->getConfiguration());
      // Declaration of the device and Associated keywords
      declareDevice(api);

      // the main loop
      std::cout << "MailSender plugin is running..." << std::endl;

      while (1)
      {
         // Wait for an event
         switch (api->getEventHandler().waitForEvents())
         {
         case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());
               break;
            }

         case yApi::IYPluginApi::kEventDeviceCommand:
            {
               // Command received
               auto command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();
               std::cout << "Command received :" << yApi::IDeviceCommand::toString(command) << std::endl;

               if (boost::iequals(command->getKeyword(), m_messageKeyword->getKeyword()))
                  onSendMailRequest(api, command->getBody());
               else
                  std::cout << "Received command for unknown keyword from Yadoms : " << yApi::IDeviceCommand::toString(command) << std::endl;
            }
            break;

         default:
            {
               std::cerr << "Unknown message id" << std::endl;
               break;
            }
         }
      }
   }
   // Plugin must catch this end-of-thread exception to make its cleanup.
   // If no cleanup is necessary, still catch it, or Yadoms will consider
   // as a plugin failure.
   catch (boost::thread_interrupted&)
   {
      std::cout << "MailSender is stopping..." << std::endl;
   }
}

void CMailSender::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (api->deviceExists(m_deviceName))
      return;

   api->declareDevice(m_deviceName, m_deviceName, m_messageKeyword);
}

void CMailSender::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api, const shared::CDataContainer& newConfigurationData) const
{
   // Configuration was updated
   std::cout << "Configuration was updated..." << std::endl;
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration->initializeWith(newConfigurationData);
}

void CMailSender::onSendMailRequest(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& sendMailRequest)
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

      boost::shared_ptr<ISmtpServiceProvider> mailServiceProvider = CSmtpServiceProviderFactory::CreateSmtpServer(m_configuration);
      if (mailServiceProvider->sendMail(message))
      {
         std::cout << "Email successfully sent to " << to << std::endl;
      }
      else
      {
         std::cerr << "Fail to send email to " << to << std::endl;
      }
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      std::cerr << "Invalid Mail sending request \"" << sendMailRequest << "\" : " << e.what() << std::endl;
      return;
   }
   catch (...)
   {
      std::cerr << "Error sending Mail" << std::endl;
   }
}


std::string CMailSender::getRecipientMail(boost::shared_ptr<yApi::IYPluginApi> api, int recipientId) const
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


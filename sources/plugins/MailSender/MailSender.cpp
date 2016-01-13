#include "stdafx.h"
#include "MailSender.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/historization/Message.h>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/ImplementationHelper.h>
#include <shared/exception/EmptyResult.hpp>
#include "EmptyParameterException.hpp"
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>
#include "SmtpServiceProviderFactory.h"
#include <Poco/Net/NetException.h>
#include "SecurityMode.h"
#include "MSConfiguration.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CMailSender)

CMailSender::CMailSender() : m_deviceName("MailSender"), m_configuration(new CMSConfiguration()),
m_mailId("email"),
m_messageKeyword("message", m_mailId, yApi::EKeywordAccessMode::kGetSet)
{
}

CMailSender::~CMailSender()
{}

void CMailSender::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
{
   try
   {
      YADOMS_LOG(information) << "MailSender is starting...";


      m_configuration->initializeWith(context->getConfiguration());
      // Declaration of the device and Associated keywords
      declareDevice(context);

      // the main loop
      YADOMS_LOG(information) << "MailSender plugin is running...";

      while (1)
      {
         // Wait for an event
         switch (context->getEventHandler().waitForEvents())
         {
            case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               onUpdateConfiguration(context, context->getEventHandler().getEventData<shared::CDataContainer>());
               break;
            }

            case yApi::IYPluginApi::kEventDeviceCommand:
            {
               // Command received
               boost::shared_ptr<const yApi::IDeviceCommand> command = context->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();
               YADOMS_LOG(debug) << "Command received :" << command->toString();

               if (boost::iequals(command->getKeyword(), m_messageKeyword.getKeyword()))
                  onSendMailRequest(context, command->getBody());
               else
                  YADOMS_LOG(warning) << "Received command for unknown keyword from Yadoms : " << command->toString();
            }
            break;

            default:
            {
               YADOMS_LOG(error) << "Unknown message id";
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
      YADOMS_LOG(information) << "MailSender is stopping..." << std::endl;
   }
}

void CMailSender::declareDevice(boost::shared_ptr<yApi::IYPluginApi> context)
{
   if (context->deviceExists(m_deviceName))
      return;

   // Declare the device
   context->declareDevice(m_deviceName, m_deviceName);

   // Declare associated keywords (= values managed by this device)
   context->declareKeyword(m_deviceName, m_messageKeyword);
}

void CMailSender::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> context, const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   YADOMS_LOG(debug) << "Configuration was updated...";
   BOOST_ASSERT(!newConfigurationData.empty());  // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration->initializeWith(newConfigurationData);
}

void CMailSender::onSendMailRequest(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& sendMailRequest)
{
   try
   {
      m_messageKeyword.setCommand(sendMailRequest);

      std::string from = m_configuration->getSenderEmail();
      std::string to = getRecipientMail(context, m_messageKeyword.to());

      std::string subject = "Yadoms Notification !";
      Poco::Net::MailMessage message;
      message.setSender(from);
      message.addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT, to));
      message.setSubject(subject);
      message.setContentType("text/plain; charset=UTF-8");
      message.setContent(m_messageKeyword.body(), Poco::Net::MailMessage::ENCODING_8BIT);

      boost::shared_ptr<ISmtpServiceProvider>  mailServiceProvider = CSmtpServiceProviderFactory::CreateSmtpServer(m_configuration);
      if (mailServiceProvider->sendMail(message))
      {
         YADOMS_LOG(debug) << "Email successfully sent to " << to;
      }
      else
      {
         YADOMS_LOG(error) << "Fail to send email to " << to;
      }
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Invalid Mail sending request \"" << sendMailRequest << "\" : " << e.what();
      return;
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Error sending Mail";
   }
}


std::string CMailSender::getRecipientMail(boost::shared_ptr<yApi::IYPluginApi> context, int recipientId)
{
   try
   {
      return context->getRecipientValue(recipientId, m_mailId);
   }
   catch (shared::exception::CEmptyResult& e)
   {
      throw shared::exception::CInvalidParameter(std::string("Recipient not found, or e-mail not found for recipient : ") + e.what());
   }
}

#include "stdafx.h"
#include "FreeMobileSMS.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include "SmsKeyword.h"

#include <Poco/URI.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <shared/plugin/yPluginApi/historization/MessageFormatter.h>
#include <shared/Log.h>

IMPLEMENT_PLUGIN(CFreeMobileSms)

std::string CFreeMobileSms::m_deviceName("sms");
std::string CFreeMobileSms::m_keywordName("sms");
std::string CFreeMobileSms::m_freeMobileApiUrl("https://smsapi.free-mobile.fr/sendmsg?user=%1%&pass=%2%&msg=%3%");


CFreeMobileSms::CFreeMobileSms()
{
}

CFreeMobileSms::~CFreeMobileSms()
{
}

void CFreeMobileSms::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   //create device/keyword
   auto keyword(boost::make_shared<CSmsKeyword>(m_keywordName));

   if (!api->deviceExists(m_deviceName))
      api->declareDevice(m_deviceName, "FreeMobile SMS Api");

   // Declare these sensors to Yadoms (devices and associated keywords) if not already declared
   if (!api->keywordExists(m_deviceName, keyword))
      api->declareKeyword(m_deviceName, keyword);

   // the main loop
   YADOMS_LOG(information) << "CFreeMobileSms is running..." ;
   api->setPluginState(yApi::historization::EPluginState::kRunning);
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
      case yApi::IYPluginApi::kEventDeviceCommand:
         {
            // A command was received from Yadoms
            auto command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>();
            YADOMS_LOG(information) << "Command received from Yadoms :" << yApi::IDeviceCommand::toString(command) ;

            try
            {
               //parse the command data
               yApi::historization::CMessageFormatter msgInfo(command->getBody());


               if (msgInfo.isToProvided() && msgInfo.isBodyProvided())
               {
                  //send sms
                  sendSms(api, msgInfo.to(), msgInfo.body());
               }
               else if (!msgInfo.isToProvided())
               {
                  YADOMS_LOG(error) << "SMS recipient ('to') not found in command data" << yApi::IDeviceCommand::toString(command) ;
               }
               else if (!msgInfo.isBodyProvided())
               {
                  YADOMS_LOG(error) << "SMS content ('body') not found in command data" << yApi::IDeviceCommand::toString(command) ;
               }
            }
            catch (std::exception& e)
            {
               YADOMS_LOG(error) << "Fail to send command " << yApi::IDeviceCommand::toString(command) << ", error : " << e.what();
            }
            break;
         }
      default:
         {
            YADOMS_LOG(error) << "Unknown ou unsupported message id " << api->getEventHandler().getEventId() ;
            break;
         }
      }
   }
}


void CFreeMobileSms::sendSms(boost::shared_ptr<yApi::IYPluginApi> api,
                             const int recipientId,
                             const std::string& smsContent)
{
   try
   {
      //retreive recipient parameters (login & key)
      auto userId = api->getRecipientValue(recipientId, "userId");
      auto key = api->getRecipientValue(recipientId, "apiKey");

      //format url
      auto uriStr = (boost::format(m_freeMobileApiUrl) % userId % key % smsContent).str();

      //parse url
      Poco::URI uri(uriStr);
      auto path(uri.getPathAndQuery());
      if (path.empty())
         path = "/";

      //prepare SSL session
      const Poco::Net::Context::Ptr netcontext(new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE));
      Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort(), netcontext);

      //prepare request
      Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);
      request.setKeepAlive(true);

      //send request
      Poco::Net::HTTPResponse response;
      session.sendRequest(request);

      //read answer
      session.receiveResponse(response);
      YADOMS_LOG(information) << response.getStatus() << " " << response.getReason() ;
      if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
      {
         //return true;
         YADOMS_LOG(information) << "SMS sent" ;
      }
      else
      {
         //it went wrong ?
         YADOMS_LOG(error) << "Fail to send SMS : " << response.getStatus() << " " << response.getReason() ;
      }
   }
   catch (Poco::Exception& ex)
   {
      YADOMS_LOG(error) << "Error " << ex.displayText() ;
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Error " << ex.what() ;
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Unknown exception in sendSms" ;
   }
}


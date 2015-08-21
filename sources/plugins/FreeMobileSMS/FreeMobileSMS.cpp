#include "stdafx.h"
#include "FreeMobileSMS.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/Log.h>
#include "SmsKeyword.h"

#include <Poco/URI.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPSClientSession.h>


// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values
IMPLEMENT_PLUGIN(CFreeMobileSms)

std::string CFreeMobileSms::m_deviceName("sms");
std::string CFreeMobileSms::m_keywordName("sms");

CFreeMobileSms::CFreeMobileSms()
{
}

CFreeMobileSms::~CFreeMobileSms()
{
}


void CFreeMobileSms::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
{
   try
   {
      //create device/keyword
      boost::shared_ptr< CSmsKeyword > m_keyword(new CSmsKeyword(m_keywordName));
   
      if (!context->deviceExists(m_deviceName))
         context->declareDevice(m_deviceName, "FreeMobile SMS Api");

      // Declare these sensors to Yadoms (devices and associated keywords) if not already declared
      if (!context->keywordExists(m_deviceName, *m_keyword))
         context->declareKeyword(m_deviceName, *m_keyword);

      // the main loop
      YADOMS_LOG(debug) << "CFreeMobileSms is running...";
      context->recordPluginEvent(yApi::IYPluginApi::kInfo, "started");
      while(1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case yApi::IYPluginApi::kEventDeviceCommand:
            {
               // A command was received from Yadoms
               boost::shared_ptr<const yApi::IDeviceCommand> command = context->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();
               YADOMS_LOG(debug) << "Command received from Yadoms :" << command->toString();
               
               shared::CDataContainer dc(command->getBody());
               if (dc.containsValue("recipientId"))
               {
                  //get recipient id from commandData
                  int recipientId = dc.get<int>("recipientId");

                  //retreive recipient parameters (login & key)
                  std::string userId = context->getRecipientValue(recipientId, "userId");
                  std::string key = context->getRecipientValue(recipientId, "apiKey");

                  //get message to send from commandData
                  std::string message = dc.get<std::string>("message");

                  //format url
                  std::string uriStr = (boost::format("https://smsapi.free-mobile.fr/sendmsg?user=%1%&pass=%2%&msg=%3%") % userId % key % message).str();

                  try
                  {
                     //parse url
                     Poco::URI uri(uriStr);
                     std::string path(uri.getPathAndQuery());
                     if (path.empty()) 
                        path = "/";
                     
                     //prepare SSL session
                     const Poco::Net::Context::Ptr context(new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE));
                     Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort(), context);

                     //prepare request
                     Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);
                     request.setKeepAlive(true);
                     
                     //send request
                     Poco::Net::HTTPResponse response;
                     session.sendRequest(request);
                     
                     //read answer
                     session.receiveResponse(response);
                     std::cout << response.getStatus() << " " << response.getReason() << std::endl;
                     if (response.getStatus() != Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
                     {
                        //return true;
                        YADOMS_LOG(information) << "SMS sent";
                     }
                     else
                     {
                        //it went wrong ?
                        YADOMS_LOG(error) << "Fail to send SMS : " << response.getStatus() << " " << response.getReason();
                     }
                  }
                  catch (Poco::Exception & ex)
                  {
                     YADOMS_LOG(error) << "Error " << ex.displayText();
                  }
               }
               else
               {
                  YADOMS_LOG(error) << "Recipient Id not found in command data " << command->toString();
               }
               break;
            }
         default:
            {
               YADOMS_LOG(error) << "Unknown ou unsupported message id " << context->getEventHandler().getEventId();
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
      YADOMS_LOG(information) << "Thread is stopping...";
   }

   context->recordPluginEvent(yApi::IYPluginApi::kInfo, "stopped");
}
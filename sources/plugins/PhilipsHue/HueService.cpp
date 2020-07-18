#include "HueService.h"
#include "shared/http/HttpMethods.h"
#include "shared/Log.h"
#include <boost/asio.hpp>


CHueService::CHueService(shared::event::CEventHandler& mainEventHandler, int evtKeyStateReceived,
                         int evtKeyStateTimeout)
   : m_mainEventHandler(mainEventHandler),
     m_mainEvtKeyStateReceived(evtKeyStateReceived),
     m_mainEvtKeyStateTimeout(evtKeyStateTimeout)
{
};

CHueService::~CHueService()
{
   m_readBridgeButtonStateThread->interrupt();
   m_readBridgeButtonStateThread->join();
}

void CHueService::startReadingBridgeButtonState()
{
   m_readBridgeButtonStateThread = boost::make_shared<boost::thread>(boost::bind(&CHueService::requestUsername, this));
}

void CHueService::closeReadingBridgeButtonState()
{
   m_readBridgeButtonStateThread->interrupt();
   m_readBridgeButtonStateThread->join();
}

void CHueService::requestUsername() const
{
   const auto start = std::chrono::steady_clock::now();

   while (std::chrono::steady_clock::now() - start < std::chrono::seconds(30))
   {
      try
      {
         const auto url = "http://192.168.1.14/api/";
         const auto body = buildAuthorizedUsernameBody();
         const auto headerPostParameters = buildCommonHeaderParameters();

         const auto out = shared::http::CHttpMethods::sendJsonPostRequest(url,
                                                                          body,
                                                                          headerPostParameters);

         auto authorizedUsername = out->get("0.success.username");

         if (!authorizedUsername.empty())
         {
            YADOMS_LOG(information) << "key bridge is pressed";
            m_mainEventHandler.postEvent(m_mainEvtKeyStateReceived);
         }
      }
      catch (std::exception& e)
      {
         YADOMS_LOG(error) << "Fail to send Post http request or interpret answer : " << e.what();
      }
   }
   YADOMS_LOG(information) << "key bridge is not pressed";
   m_mainEventHandler.postEvent(m_mainEvtKeyStateTimeout);
}


std::string CHueService::buildAuthorizedUsernameBody()
{
   shared::CDataContainer body;
   body.set("devicetype", "philipsHue#yadoms");
   return body.serialize();
}


std::map<std::string, std::string> CHueService::buildCommonHeaderParameters()
{
   std::map<std::string, std::string> headerParameters;
   headerParameters["User-Agent"] = "yadoms";
   headerParameters["Accept"] = "application/json";
   headerParameters["Connection"] = "close";

   return headerParameters;
}

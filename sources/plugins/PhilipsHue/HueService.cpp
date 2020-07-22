#include "HueService.h"
#include "shared/http/HttpMethods.h"
#include "shared/Log.h"
#include <boost/asio.hpp>


CHueService::CHueService(shared::event::CEventHandler& mainEventHandler,
                         int evtKeyStateReceived,
                         int evtKeyStateTimeout,
                         boost::shared_ptr<CUrlManager>& urlManager)
   : m_mainEventHandler(mainEventHandler),
     m_mainEvtKeyStateReceived(evtKeyStateReceived),
     m_mainEvtKeyStateTimeout(evtKeyStateTimeout),
     m_urlManager(urlManager)
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
   std::string authorizedUsername;
   while (std::chrono::steady_clock::now() - start < std::chrono::seconds(kDefaultTimeoutSeconds))
   {
      try
      {
         const auto urlPatternPath = m_urlManager->getUrlPatternPath(CUrlManager::kApi);
         const auto url = m_urlManager->getPatternUrl(urlPatternPath);

         const auto body = buildAuthorizedUsernameBody();
         const auto headerPostParameters = buildCommonHeaderParameters();

         const auto out = shared::http::CHttpMethods::sendJsonPostRequest(url,
                                                                          body,
                                                                          headerPostParameters);

         authorizedUsername = out->get("0.success.username");

         if (!authorizedUsername.empty())
         {
            m_urlManager->setUsername(authorizedUsername);
            YADOMS_LOG(information) << "key bridge is pressed";
            YADOMS_LOG(information) << "Authorized username is : " << authorizedUsername;
            m_mainEventHandler.postEvent(m_mainEvtKeyStateReceived);
            break;
         }
      }
      catch (std::exception& e)
      {
         YADOMS_LOG(error) << "Fail to send Post http request or interpret answer : " << e.what();
      }
   }
   if (authorizedUsername.empty())
   {
      YADOMS_LOG(information) << "key bridge is not pressed";
      m_mainEventHandler.postEvent(m_mainEvtKeyStateTimeout);
   }
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

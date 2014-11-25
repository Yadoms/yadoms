#include "stdafx.h"
#include "WebSocketRequestHandler.h"

#include <shared/Log.h>
#include <shared/DataContainer.h>
#include <shared/notification/NotificationCenter.h>

#include "notifications/AsyncNotificationCenter.h"
#include "notifications/NewAcquisitionNotification.h"
#include "web/ws/FrameFactory.h"
#include "web/ws/AcquisitionFilterFrame.h"
#include "web/ws/AcquisitionUpdateFrame.h"
#include "web/ws/NewDeviceFrame.h"

#include "WebSocketClient.h"

namespace web { namespace poco {

CWebSocketRequestHandler::CWebSocketRequestHandler(boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter)
   :m_notificationCenter(notificationCenter)
{
}

CWebSocketRequestHandler::~CWebSocketRequestHandler()
{
}


void CWebSocketRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
   //for each request (each time a new ws connexion is made
   //then just create a websocket server and wait infinite (until client ends)
   try
   {
      std::vector<int> acquisitionKeywordFilters;

      boost::shared_ptr< shared::notification::CNotificationObserver> observer = m_notificationCenter->registerObserver(this);
      
      const int kNotifFromWsClient = shared::notification::CNotificationCenter::kUserFirstId;
      CWebSocketClient client(request, response, observer, kNotifFromWsClient);

      bool clientSeemConnected = true;

      while (clientSeemConnected)
      {
         switch (m_notificationCenter->waitForNotifications(this))
         {
         case kNotifFromWsClient:
         {
            boost::shared_ptr<web::ws::CFrameBase> parsedFrame = m_notificationCenter->getNotificationData< boost::shared_ptr<web::ws::CFrameBase> >(this);
            if (parsedFrame)
            {
               switch (parsedFrame->getType())
               {
               case web::ws::CFrameBase::EFrameType::kAcquisitionFilterValue:
                  boost::shared_ptr<web::ws::CAcquisitionFilterFrame> parsedFrameAsqFilter = boost::dynamic_pointer_cast<web::ws::CAcquisitionFilterFrame>(parsedFrame);
                  acquisitionKeywordFilters.clear();
                  acquisitionKeywordFilters = parsedFrameAsqFilter->getFilter();
                  break;
               }
            }

            break;
         }

         case shared::notification::CNotificationCenter::kNotification:
         {
            //a new notification has arrived
            bool somethingToSend = false;
            std::string dataString;

            //check if notification is a newAcquisition
            if (m_notificationCenter->isNotificationTypeOf< boost::shared_ptr<notifications::CNewAcquisitionNotification> >(this))
            {
               boost::shared_ptr<notifications::CNewAcquisitionNotification> newAcquisition = m_notificationCenter->getNotificationData< boost::shared_ptr<notifications::CNewAcquisitionNotification> >(this);
               if (acquisitionKeywordFilters.empty() ||
                  std::find(acquisitionKeywordFilters.begin(), acquisitionKeywordFilters.end(), newAcquisition->getAcquisition()->KeywordId()) != acquisitionKeywordFilters.end())
               {
                  web::ws::CAcquisitionUpdateFrame toSend(newAcquisition);
                  dataString = toSend.serialize();
                  somethingToSend = true;
               }
            }

            //check if notification is a newDevice
            else if (m_notificationCenter->isNotificationTypeOf< boost::shared_ptr<notifications::CNewDeviceNotification> >(this))
            {
               boost::shared_ptr<notifications::CNewDeviceNotification> newDevice = m_notificationCenter->getNotificationData< boost::shared_ptr<notifications::CNewDeviceNotification> >(this);

               web::ws::CNewDeviceFrame toSend(newDevice);
               dataString = toSend.serialize();
               somethingToSend = true;
            }

            //notifiy client
            if (somethingToSend)
            {
               int sentBytes = client.sendFrame(dataString.c_str(), dataString.length(), Poco::Net::WebSocket::FRAME_TEXT);
               if (sentBytes == 0)
                  clientSeemConnected = false;
            }

            //this notification is ignored
            break;
         }

         default:
            YADOMS_LOG(error) << "Unknown message id";
            //BOOST_ASSERT(false);
            break;
         }
      } //while
   }
   catch (shared::exception::CException & ex)
   {
      YADOMS_LOG(error) << "Websocket request handler exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Websocket request handler unknown exception";
   }

   //unregister for notifications
   m_notificationCenter->unregisterObserver(this);

}

} } //namespace web::poco


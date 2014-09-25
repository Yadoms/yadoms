#include "stdafx.h"
#include "WebSocketRequestHandler.h"

#include <Poco/Net/NetException.h>
#include <Poco/Net/WebSocket.h>

#include <shared/Log.h>
#include <shared/DataContainer.h>
#include <shared/notification/NotificationCenter.h>

#include "notifications/AsyncNotificationCenter.h"
#include "notifications/NewAcquisitionNotification.h"
#include "web/ws/FrameFactory.h"
#include "web/ws/AcquisitionFilterFrame.h"
#include "web/ws/AcquisitionUpdateFrame.h"

namespace web {
   namespace poco {

      void CWebSocketRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
      {
         //for each request (each time a new ws connexion is made
         //them just create a websocket server and wait infinite (until client ends)
         try
         {
            Poco::Net::WebSocket socketServer(request, response);
            std::vector<int> acquisitionKeywordFilters;

            notifications::CAsyncNotificationCenter::get()->registerObserver(this);

            bool clientSeemConnected = true;

            while (clientSeemConnected)
            {
               switch (notifications::CAsyncNotificationCenter::get()->waitForNotifications(this, boost::posix_time::milliseconds(100)))
               {
               case shared::notification::CNotificationCenter::kTimeout:
                  //if some data are available, then read them and answer
                  if (socketServer.available() > 0)
                  {
                     int flags = 0;
                     char buffer[2048];
                     memset(buffer, 0, 2048);

                     int n = socketServer.receiveFrame(buffer, sizeof(buffer), flags);
                     if (n > 0)
                     {
                        std::string bufferString(buffer);
                        boost::shared_ptr<web::ws::CFrameBase> parsedFrame = web::ws::CFrameFactory::tryParse(bufferString);
                        if (parsedFrame)
                        {
                           switch (parsedFrame->getType())
                           {
                           case web::ws::CFrameBase::EFrameType::kAcquisitionFilter:
                              boost::shared_ptr<web::ws::CAcquisitionFilterFrame> parsedFrameAsqFilter = boost::dynamic_pointer_cast<web::ws::CAcquisitionFilterFrame>(parsedFrame);
                              acquisitionKeywordFilters.clear();
                              acquisitionKeywordFilters = parsedFrameAsqFilter->getFilter();
                              break;
                           }
                        }
                     }
                  }

                  break;

               case shared::notification::CNotificationCenter::kNotification:
                  //a new notification has arrived

                  //check if notification is a newAcquisition
                  if (notifications::CAsyncNotificationCenter::get()->isNotificationTypeOf< boost::shared_ptr<database::entities::CAcquisition> >(this))
                  {
                     boost::shared_ptr<database::entities::CAcquisition> newAcquisition = notifications::CAsyncNotificationCenter::get()->getNotificationData< boost::shared_ptr<database::entities::CAcquisition> >(this);
                     if (acquisitionKeywordFilters.empty() ||
                        std::find(acquisitionKeywordFilters.begin(), acquisitionKeywordFilters.end(), newAcquisition->KeywordId()) != acquisitionKeywordFilters.end())
                     {
                        web::ws::CAcquisitionUpdateFrame toSend(newAcquisition);
                        std::string dataString = toSend.serialize();
                        int sentBytes = socketServer.sendFrame(dataString.c_str(), dataString.length(), Poco::Net::WebSocket::FRAME_TEXT);
                        if (sentBytes == 0)
                           clientSeemConnected = false;
                     }
                  }

                  //this notification is ignored
                  break;

               default:
                  YADOMS_LOG(error) << "Unknown message id";
                  BOOST_ASSERT(false);
                  break;
               }
            } //while
         }
         catch (Poco::Net::WebSocketException& exc)
         {
            YADOMS_LOG(error) << "Websocket request handler Poco::Net::WebSocketException";
            switch (exc.code())
            {
            case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
               response.set("Sec-WebSocket-Version", Poco::Net::WebSocket::WEBSOCKET_VERSION);
               // fallthrough
            case Poco::Net::WebSocket::WS_ERR_NO_HANDSHAKE:
            case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
            case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
               response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
               response.setContentLength(0);
               response.send();
               break;
            }
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
         notifications::CAsyncNotificationCenter::get()->unregisterObserver(this);

      }


   } //namespace poco
} //namespace web


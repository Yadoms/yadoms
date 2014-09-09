#include "stdafx.h"
#include "WebSocketRequestHandler.h"

#include <Poco/Net/NetException.h>

#include "notifications/AsyncNotificationCenter.h"
#include <shared/Log.h>
#include <shared/DataContainer.h>
#include <shared/notification/NotificationCenter.h>

namespace web {
   namespace poco {

      void CWebSocketRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
      {
         //for each request (each time a new ws connexion is made
         //them just create a websocket server and wait infinite (until client ends)
         try
         {
            m_socketServer.assign(new Poco::Net::WebSocket(request, response));
            //notifications::CAsyncNotificationCenter::get()->addObserver(Poco::NObserver<CWebSocketRequestHandler, notifications::CNewAcquisitionNotification>               (*this, &CWebSocketRequestHandler::handleNewData));
            notifications::CAsyncNotificationCenter::get()->registerObserver(this);

            while (1)
            {
               switch (notifications::CAsyncNotificationCenter::get()->waitForNotifications(this, boost::posix_time::milliseconds(100)))
               {
               case shared::notification::CNotificationCenter::kTimeout:
                  if (m_socketServer->available() > 0)
                  {
                     int flags = 0;
                     char buffer[2048];
                     memset(buffer, 0, 2048);

                     int n = m_socketServer->receiveFrame(buffer, sizeof(buffer), flags);
                     if (n > 0)
                     {
                        std::string bufferString(buffer);
                        shared::CDataContainer obj(bufferString);

                        if (obj.get<std::string>("type") == "acquisitionFilter")
                        {
                           std::vector<int> filter = obj.get< std::vector<int> >("data");
                           updateFilters(filter);
                        }
                     }
                  }

                  break;

               case shared::notification::CNotificationCenter::kNotification:
                  if (notifications::CAsyncNotificationCenter::get()->isNotificationTypeOf<database::entities::CAcquisition>(this))
                  {
                     database::entities::CAcquisition newAcquisition = notifications::CAsyncNotificationCenter::get()->getNotificationData<database::entities::CAcquisition>(this);
                     if (m_acquisitionKeywordFilters.empty() || 
                        std::find(m_acquisitionKeywordFilters.begin(), m_acquisitionKeywordFilters.end(), newAcquisition.KeywordId()) != m_acquisitionKeywordFilters.end())
                     {
                        shared::CDataContainer toSend;
                        toSend.set<std::string>("type", "acquisitionUpdate");
                        toSend.set("data", newAcquisition);

                        std::string dataString = toSend.serialize();
                        int sentBytes = m_socketServer->sendFrame(dataString.c_str(), dataString.length(), Poco::Net::WebSocket::FRAME_TEXT);
                        if (sentBytes == 0)
                           finalizeServer();
                     }
                  }
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
         catch (...)
         {

         }


      }

      void CWebSocketRequestHandler::updateFilters(std::vector<int> & newFilters)
      {
         m_acquisitionKeywordFilters = newFilters;
      }


      void CWebSocketRequestHandler::finalizeServer()
      {
         //notifications::CAsyncNotificationCenter::get()->removeObserver(Poco::NObserver<CWebSocketRequestHandler, notifications::CNewAcquisitionNotification>(*this, &CWebSocketRequestHandler::handleNewData));
         notifications::CAsyncNotificationCenter::get()->unregisterObserver(this);
      }


   } //namespace poco
} //namespace web


#include "stdafx.h"
#include "WebSocketRequestHandler.h"

#include <Poco/Net/NetException.h>
#include <Poco/NObserver.h>

#include <Poco/Thread.h>

#include "notifications/AsyncNotificationCenter.h"
#include "shared/Log.h"
#include <shared/DataContainer.h>

namespace web { namespace poco {

      void CWebSocketRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
      {
         //for each request (each time a new ws connexion is made
         //them just create a websocket server and wait infinite (until client ends)
         try
         {
            m_socketServer.assign(new Poco::Net::WebSocket(request, response));
            notifications::CAsyncNotificationCenter::get()->addObserver(Poco::NObserver<CWebSocketRequestHandler, notifications::CNewAcquisitionNotification>
               (*this, &CWebSocketRequestHandler::handleNewData));
            //TODO : envoyer un PING/PONG avec le client pour s'assurer que l'on communique correctement (cf catch)
            /*
            char buffer[1024];
            int flags;
            int n;
            do
            {
               boost::this_thread::sleep(boost::posix_time::milliseconds(500));
               m_socketServer->sendBytes(NULL, 0, Poco::Net::WebSocket::FRAME_OP_PING);
               n = m_socketServer->receiveFrame(buffer, sizeof(buffer), flags);
            } while (n > 0 || (flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) != Poco::Net::WebSocket::FRAME_OP_CLOSE);
            */

            int flags;
            char buffer[2048];
            int n;

            do
            {
               /*
               {
                  "type" : "acquisitionFilter",
                     "data" : [2, 4]
               }
               */
               n = m_socketServer->receiveFrame(buffer, sizeof(buffer), flags);
               
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

               //boost::this_thread::sleep(boost::posix_time::milliseconds(500));
            } while (n != 0);
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
         notifications::CAsyncNotificationCenter::get()->removeObserver(Poco::NObserver<CWebSocketRequestHandler, notifications::CNewAcquisitionNotification>(*this, &CWebSocketRequestHandler::handleNewData));
      }

      void CWebSocketRequestHandler::handleNewData(const Poco::AutoPtr<notifications::CNewAcquisitionNotification> & newAcquisition)
      {
         boost::recursive_timed_mutex::scoped_lock lock(m_mutex);

         bool isAcquisitionFiltered = !m_acquisitionKeywordFilters.empty(); //if empty, acq is not filtered
         std::vector<int>::iterator i;
         for (i = m_acquisitionKeywordFilters.begin(); i != m_acquisitionKeywordFilters.end(); ++i)
         {
            if (*i == newAcquisition->getAcquisition()->Id())
            {
               isAcquisitionFiltered = false;
               break;
            }
         }

         
         if (!isAcquisitionFiltered)
         {
            shared::CDataContainer toSend;
            toSend.set<std::string>("type", "acquisitionUpdate");
            toSend.set("data", newAcquisition->getAcquisition());

            std::string dataString = toSend.serialize();
            int sentBytes = m_socketServer->sendFrame(dataString.c_str(), dataString.length(), Poco::Net::WebSocket::FRAME_TEXT);
            if (sentBytes == 0)
               finalizeServer();
         }
      }

} //namespace poco
} //namespace web


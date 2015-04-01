#include "stdafx.h"
#include "WebSocketClient.h"
#include <Poco/Net/NetException.h>
#include "web/ws/FrameFactory.h"
#include <shared/Log.h>

namespace web { namespace poco {


   CWebSocketClient::CWebSocketClient(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, boost::shared_ptr<shared::notification::CNotificationObserver> observer, const int observerEvent)
      :CThreadBase("WebSocketClient"), m_observer(observer), m_observerEvent(observerEvent), m_socket(request, response), m_response(response)
   {

   }
   
   CWebSocketClient::~CWebSocketClient()
   {
   }

   int CWebSocketClient::sendFrame(const void* buffer, int length, int flags)
   {
      return m_socket.sendFrame(buffer, length, flags);
   }
   

   void CWebSocketClient::doWork()
   {
      try
      {
         //if some data are available, then read them and answer
         while (m_socket.available() >= 0 && !this->isStopping())
         {
            int flags = 0;
            char buffer[2048] = { 0 };

            int n = m_socket.receiveFrame(buffer, sizeof(buffer), flags);
            if (n > 0)
            {
               std::string bufferString(buffer);
               boost::shared_ptr<web::ws::CFrameBase> parsedFrame = web::ws::CFrameFactory::tryParse(bufferString);
               if (parsedFrame)
               {
                  m_observer->postNotification(m_observerEvent, parsedFrame);
               }
            }
         }
      }
      catch (Poco::Net::WebSocketException& exc)
      {
         YADOMS_LOG(error) << "Websocket request handler Poco::Net::WebSocketException";
         switch (exc.code())
         {
         case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
            m_response.set("Sec-WebSocket-Version", Poco::Net::WebSocket::WEBSOCKET_VERSION);
            // fallthrough
         case Poco::Net::WebSocket::WS_ERR_NO_HANDSHAKE:
         case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
         case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
            m_response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
            m_response.setContentLength(0);
            m_response.send();
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
   }

} //namespace poco
} //namespace web


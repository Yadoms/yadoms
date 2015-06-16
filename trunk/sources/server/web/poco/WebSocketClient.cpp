#include "stdafx.h"
#include "WebSocketClient.h"
#include <Poco/Net/NetException.h>
#include "web/ws/FrameFactory.h"
#include <shared/Log.h>

namespace web { namespace poco {


   CWebSocketClient::CWebSocketClient(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, shared::event::CEventHandler& eventHandler, int eventId)
      :CThreadBase("WebSocketClient"), m_eventHandler(eventHandler), m_eventId(eventId), m_socket(request, response), m_response(response)
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
               boost::shared_ptr<ws::CFrameBase> parsedFrame = ws::CFrameFactory::tryParse(bufferString);
               if (parsedFrame)
                  m_eventHandler.postEvent(m_eventId, parsedFrame);
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
         default:
            m_response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
            m_response.setContentLength(0);
            m_response.send();
            break;
         }
      }
      catch (Poco::IOException& exc)
      {
         YADOMS_LOG(warning) << "Websocket request handler Poco::IOException : " << exc.what();
      }
      catch (shared::exception::CException & ex)
      {
         YADOMS_LOG(warning) << "Websocket request handler exception : " << ex.what();
      }
      catch (...)
      {
         YADOMS_LOG(error) << "Websocket request handler unknown exception";
      }
   }

} //namespace poco
} //namespace web


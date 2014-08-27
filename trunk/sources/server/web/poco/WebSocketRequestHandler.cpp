#include "stdafx.h"
#include "WebSocketRequestHandler.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"


namespace web { namespace poco {


      void CWebSocketRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
      {
         try
         {
            Poco::Net::WebSocket ws(request, response);
            char buffer[1024];
            int flags;
            int n;
            do
            {
               n = ws.receiveFrame(buffer, sizeof(buffer), flags);
               ws.sendFrame(buffer, n, flags);
            } while (n > 0 || (flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) != Poco::Net::WebSocket::FRAME_OP_CLOSE);
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
      }
} //namespace poco
} //namespace web


#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include "WebSocketClient.h"
#include "web/ws/FrameBase.h"

namespace web { namespace poco {

   class CWebSocketRequestHandler : public Poco::Net::HTTPRequestHandler
   {
   public:
      CWebSocketRequestHandler();
      virtual ~CWebSocketRequestHandler();

      void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
      
      //--------------------------------------------------------------
      /// \brief           Send data on the webSocket
      /// \param[in] client Web socket client
      /// \param[in] toSend Data to send
      /// \return true is sending OK, false if connection lost
      //--------------------------------------------------------------
      bool send(CWebSocketClient& client, const ws::CFrameBase& toSend) const;
   };


} //namespace poco
} //namespace web


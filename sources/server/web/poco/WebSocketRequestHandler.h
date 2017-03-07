#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/WebSocket.h>

#include "web/ws/FrameBase.h"

namespace web
{
   namespace poco
   {
      class CWebSocketRequestHandler : public Poco::Net::HTTPRequestHandler
      {
      public:
         CWebSocketRequestHandler();
         virtual ~CWebSocketRequestHandler();

         void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;

      private:
         //--------------------------------------------------------------
         /// \brief           Send data on the webSocket
         /// \param[in] client Web socket client
         /// \param[in] toSend Data to send
         /// \return true is sending OK, false if connection lost
         //--------------------------------------------------------------
         static bool send(Poco::Net::WebSocket& webSocket, const ws::CFrameBase& toSend);

         //--------------------------------------------------------------
         /// \brief           Send PING frame on the webSocket
         /// \param[in] client Web socket client
         /// \return true is sending OK, false if connection lost
         //--------------------------------------------------------------
         static bool sendPing(Poco::Net::WebSocket& webSocket);
      };
   } //namespace poco
} //namespace web



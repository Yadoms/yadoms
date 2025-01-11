#pragma once
#include <oatpp/web/server/HttpRequestHandler.hpp>
#include <oatpp-websocket/ConnectionHandler.hpp>

namespace web
{
   namespace oatppServer
   {
      class CWebsocketRequestHandler final : public oatpp::web::server::HttpRequestHandler
      {
      public:
         explicit CWebsocketRequestHandler(std::shared_ptr<oatpp::websocket::ConnectionHandler> websocketConnectionHandler);
         ~CWebsocketRequestHandler() override = default;

         // oatpp::web::server::HttpRequestHandler Implementation
         std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override;
         // [END] oatpp::web::server::HttpRequestHandler Implementation

      private:
         std::shared_ptr<oatpp::websocket::ConnectionHandler> m_websocketConnectionHandler;
      };
   } //namespace oatppServer
} //namespace web

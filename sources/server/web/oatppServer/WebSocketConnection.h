#pragma once
#include <oatpp-websocket/ConnectionHandler.hpp>

namespace web
{
   namespace oatppServer
   {
      class CWebSocketConnection : public oatpp::websocket::ConnectionHandler::SocketInstanceListener
      {
      public:
         virtual ~CWebSocketConnection() = default;

         void onAfterCreate(const oatpp::websocket::WebSocket& socket,
                            const std::shared_ptr<const ParameterMap>& params) override;
         void onBeforeDestroy(const oatpp::websocket::WebSocket& socket) override;

      private:
         static std::atomic<v_int32> m_clientsCount;
      };
   } //namespace oatppServer
} //namespace web

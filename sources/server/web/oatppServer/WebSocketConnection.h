#pragma once
#include <oatpp-websocket/ConnectionHandler.hpp>

#include "shared/event/EventHandler.hpp"

namespace web
{
   namespace oatppServer
   {
      class CWebSocketConnection final : public oatpp::websocket::ConnectionHandler::SocketInstanceListener
      {
      public:
         virtual ~CWebSocketConnection() = default;

         void onAfterCreate(const oatpp::websocket::WebSocket& socket,
                            const std::shared_ptr<const ParameterMap>& params) override;
         void onBeforeDestroy(const oatpp::websocket::WebSocket& socket) override;
         static void sendMessage(const std::string& message, const WebSocket& socket);

      private:
         static void sendTimeSynchronization(const boost::posix_time::ptime& time,
                                             const WebSocket& socket);
         void handleConnectionThread(const oatpp::websocket::WebSocket& socket);

         static std::atomic<v_int32> m_clientsCount;
         boost::thread m_connectionThread;
         shared::event::CEventHandler m_eventHandler;
      };
   } //namespace oatppServer
} //namespace web

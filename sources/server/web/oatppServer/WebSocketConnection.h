#pragma once
#include <oatpp-websocket/ConnectionHandler.hpp>

#include "shared/DataContainer.h"
#include "shared/event/EventHandler.hpp"

namespace web
{
   namespace oatppServer
   {
      class CWebSocketConnection final : public oatpp::websocket::ConnectionHandler::SocketInstanceListener
      {
      public:
         void onAfterCreate(const oatpp::websocket::WebSocket& socket,
                            const std::shared_ptr<const ParameterMap>& params) override;
         void onBeforeDestroy(const oatpp::websocket::WebSocket& socket) override;

         void close();

      private:
         static void sendMessage(const std::string& message, const WebSocket& socket);
         std::unique_ptr<shared::CDataContainer> makeNewAcquisitionContainer() const;
         static std::string makeIsAliveReply();
         static void sendTimeSynchronization(const boost::posix_time::ptime& time,
                                             const WebSocket& socket);
         void handleConnectionThread(const oatpp::websocket::WebSocket& socket);

         static std::atomic<v_int32> m_clientsCount;
         boost::thread m_connectionThread;
         shared::event::CEventHandler m_eventHandler;
      };
   } //namespace oatppServer
} //namespace web

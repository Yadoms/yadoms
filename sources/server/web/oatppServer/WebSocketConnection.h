#pragma once
#include <oatpp-websocket/ConnectionHandler.hpp>

#include "database/entities/Entities.h"
#include "shared/event/EventHandler.hpp"
#include "shared/http/ssdp/Client.h"

namespace web
{
   namespace oatppServer
   {
      class CWebSocketConnection final : public oatpp::websocket::ConnectionHandler::SocketInstanceListener //TODO virer ? (remplacé par SSE)
      {
      public:
         void onAfterCreate(const oatpp::websocket::WebSocket& socket,
                            const std::shared_ptr<const ParameterMap>& params) override;
         void onBeforeDestroy(const oatpp::websocket::WebSocket& socket) override;

         void close();

      private:
         static void sendMessage(const std::string& message, const WebSocket& socket);
         static void sendTimeSynchronization(const boost::posix_time::ptime& time,
                                             const WebSocket& socket);
         static void sendNewAcquisition(const boost::shared_ptr<database::entities::CAcquisition>& newAcquisition,
                                        const WebSocket& socket);
         static void sendDeviceCreated(const boost::shared_ptr<database::entities::CDevice>& device,
                                       const WebSocket& socket);
         static void sendDeviceDeleted(const boost::shared_ptr<database::entities::CDevice>& device,
                                       const WebSocket& socket);
         static void sendKeywordCreated(const boost::shared_ptr<database::entities::CKeyword>& keyword,
                                        const WebSocket& socket);
         static void sendKeywordDeleted(const boost::shared_ptr<database::entities::CKeyword>& keyword,
                                        const WebSocket& socket);
         void handleConnectionThread(const oatpp::websocket::WebSocket& socket);

         static std::atomic<v_int32> m_clientsCount;
         boost::thread m_connectionThread;
         shared::event::CEventHandler m_eventHandler;
      };
   } //namespace oatppServer
} //namespace web

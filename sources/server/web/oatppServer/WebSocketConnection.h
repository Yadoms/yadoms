#pragma once
#include <oatpp-websocket/ConnectionHandler.hpp>

#include "database/entities/Entities.h"
#include "shared/http/ssdp/Client.h"

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
         static void sendTimeSynchronization(const boost::posix_time::ptime& time,
                                             const WebSocket& socket);
         static void sendNewAcquisition(const boost::shared_ptr<database::entities::CAcquisition>& newAcquisition,
                                        const WebSocket& socket);
         static void sendNewAcquisitionSummary(const std::vector<boost::shared_ptr<database::entities::CAcquisitionSummary>>& newAcquisitionSummary,
                                               const WebSocket& socket);
         static void sendDeviceCreated(const boost::shared_ptr<database::entities::CDevice>& device,
                                       const WebSocket& socket);
         static void sendDeviceDeleted(const boost::shared_ptr<database::entities::CDevice>& device,
                                       const WebSocket& socket);
         static void sendKeywordCreated(const boost::shared_ptr<database::entities::CKeyword>& keyword,
                                        const WebSocket& socket);
         static void sendKeywordDeleted(const boost::shared_ptr<database::entities::CKeyword>& keyword,
                                        const WebSocket& socket);
         static void handleConnectionThread(const oatpp::websocket::WebSocket& socket);

         static std::atomic_uint m_clientsCount;

         // Because oatpp::websocket::WebSocket and CWebSocketConnection have both their thread,
         // keep track of association to be able to stop both threads when a connection is lost or
         // Yadoms stops.
         std::recursive_mutex m_connectionThreadsMutex;
         std::map<const oatpp::websocket::WebSocket*, boost::thread> m_connectionThreads;

      };
   } //namespace oatppServer
} //namespace web
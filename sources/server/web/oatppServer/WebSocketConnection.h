#pragma once
#include <oatpp-websocket/ConnectionHandler.hpp>

#include "WebsocketOnNewAcquisitionHandler.h"
#include "dataAccessLayer/IDataAccessLayer.h"
#include "shared/http/ssdp/Client.h"

namespace web
{
   namespace oatppServer
   {
      class CWebSocketConnection : public oatpp::websocket::ConnectionHandler::SocketInstanceListener
      {
      public:
         explicit CWebSocketConnection(boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer);

         virtual ~CWebSocketConnection() = default;

         void onAfterCreate(const oatpp::websocket::WebSocket& socket,
                            const std::shared_ptr<const ParameterMap>& params) override;
         void onBeforeDestroy(const oatpp::websocket::WebSocket& socket) override;

      private:
         static std::atomic<v_int32> m_clientsCount;
         boost::shared_ptr<dataAccessLayer::IDataAccessLayer> m_dataAccessLayer;
         boost::shared_ptr<CWebsocketOnNewAcquisitionHandler> m_acquisitionObserver;
      };
   } //namespace oatppServer
} //namespace web

#include "stdafx.h"
#include "WebsocketInstanceListener.h"
#include "WebsocketListener.h"

#include "shared/Log.h"

namespace web
{
   namespace oatppServer
   {
      std::atomic<v_int32> CWebsocketInstanceListener::m_clientsCount(0);

      void CWebsocketInstanceListener::onAfterCreate(const oatpp::websocket::WebSocket& socket,
                                                     const std::shared_ptr<const ParameterMap>& params)
      {
         ++m_clientsCount;
         YADOMS_LOG(information) << "New websocket connection (Client count=" << m_clientsCount.load() << ")";

         /* In this particular case we create one CWebsocketListener per each connection */
         /* Which may be redundant in many cases */
         socket.setListener(std::make_shared<CWebsocketListener>());
      }

      void CWebsocketInstanceListener::onBeforeDestroy(const oatpp::websocket::WebSocket& socket)
      {
         --m_clientsCount;
         YADOMS_LOG(information) << "Connection closed (Client count=" << m_clientsCount.load() << ")";
      }
   } //namespace oatppServer
} //namespace web

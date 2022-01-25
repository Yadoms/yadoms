#include "stdafx.h"
#include "WebSocketConnection.h"
#include "WebsocketListener.h"

#include "shared/Log.h"

namespace web
{
   namespace oatppServer
   {
      std::atomic<v_int32> CWebSocketConnection::m_clientsCount(0);

      CWebSocketConnection::CWebSocketConnection(boost::shared_ptr<dataAccessLayer::IDataAccessLayer> dataAccessLayer)
         : m_dataAccessLayer(std::move(dataAccessLayer))
      {
      }

      void CWebSocketConnection::onAfterCreate(const oatpp::websocket::WebSocket& socket,
                                               const std::shared_ptr<const ParameterMap>& params)
      {
         ++m_clientsCount;
         YADOMS_LOG(information) << "New websocket connection (Client count=" << m_clientsCount.load() << ")";

         m_acquisitionObserver = boost::make_shared<CWebsocketOnNewAcquisitionHandler>(
            [&socket](const std::string& message)
            {
               socket.sendOneFrameText(message);
            });
         m_dataAccessLayer->getAcquisitionHistorizer()->acquisitionObservers()->addObserver(m_acquisitionObserver);

         /* In this particular case we create one CWebsocketListener per each connection */
         /* Which may be redundant in many cases */
         socket.setListener(std::make_shared<CWebsocketListener>(m_acquisitionObserver));
      }

      void CWebSocketConnection::onBeforeDestroy(const oatpp::websocket::WebSocket& socket)
      {
         m_dataAccessLayer->getAcquisitionHistorizer()->acquisitionObservers()->removeObserver(m_acquisitionObserver);

         --m_clientsCount;
         YADOMS_LOG(information) << "Connection closed (Client count=" << m_clientsCount.load() << ")";

         //TODO voir pourquoi ça crashe quand une socket est connectée à la fermeture de Yadoms
      }
   } //namespace oatppServer
} //namespace web

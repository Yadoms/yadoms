#include "stdafx.h"
#include "WebsocketListener.h"

#include "web/poco/ws/FrameFactory.h"

namespace web
{
   namespace oatppServer
   {
      CWebsocketListener::CWebsocketListener(shared::event::CEventHandler& eventHandler,
                                             int onPongEventId,
                                             int onPingEventId,
                                             int onReceiveEventId)
         : m_eventHandler(eventHandler),
           m_onPongEventId(onPongEventId),
           m_onPingEventId(onPingEventId),
           m_onReceiveEventId(onReceiveEventId)
      {
         YADOMS_LOG_CONFIGURE("Websocket listener")
      }

      void CWebsocketListener::onPing(const WebSocket& socket,
                                      const oatpp::String& message)
      {
         m_eventHandler.postEvent(m_onPingEventId);
      }

      void CWebsocketListener::onPong(const WebSocket& socket,
                                      const oatpp::String& message)
      {
         m_eventHandler.postEvent(m_onPongEventId);
      }

      void CWebsocketListener::onClose(const WebSocket& socket,
                                       v_uint16 code,
                                       const oatpp::String& message)
      {
         YADOMS_LOG(debug) << "onClose, code = " << code;
      }

      void CWebsocketListener::readMessage(const WebSocket& socket,
                                           v_uint8 opcode,
                                           p_char8 data,
                                           oatpp::v_io_size size)
      {
         if (size > 0)
         {
            // message frame received

            m_messageBuffer.writeSimple(data, static_cast<v_buff_size>(size));
            return;
         }

         if (size == 0)
         {
            // message transfer finished

            const auto wholeMessage(*m_messageBuffer.toString());
            m_messageBuffer.setCurrentPosition(0);

            try
            {
               m_eventHandler.postEvent<const std::string>(m_onReceiveEventId,
                                                           wholeMessage);
            }
            catch (const std::exception&)
            {
               YADOMS_LOG(error) << "Error processing received message " << wholeMessage;
            }
         }
      }
   } //namespace oatppServer
} //namespace web

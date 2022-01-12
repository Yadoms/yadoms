#include "stdafx.h"
#include "WebsocketListener.h"

#include "shared/Log.h"

namespace web
{
   namespace oatppServer
   {
      CWebsocketListener::CWebsocketListener()
      {
         YADOMS_LOG_CONFIGURE("Websocket")
      }

      void CWebsocketListener::onPing(const WebSocket& socket,
                                      const oatpp::String& message)
      {
         YADOMS_LOG(debug) << "onPing";
         socket.sendPong(message);
      }

      void CWebsocketListener::onPong(const WebSocket& socket,
                                      const oatpp::String& message)
      {
         YADOMS_LOG(debug) << "onPong";
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

            const auto wholeMessage = m_messageBuffer.toString();
            m_messageBuffer.setCurrentPosition(0);

            YADOMS_LOG(trace) << "onMessage " << wholeMessage->c_str();

            /* Send message in reply */
//TODO à conserver (voir https://github.com/oatpp/example-websocket) ?            std::lock_guard<std::mutex> lock(m_writeMutex);
            socket.sendOneFrameText("Hello from Yadoms ! : " + wholeMessage);
         }
      }
   } //namespace oatppServer
} //namespace web

#include "stdafx.h"
#include "WebsocketListener.h"

#include "shared/Log.h"

namespace web
{
   namespace oatppServer
   {
      CWebsocketListener::CWebsocketListener()
      {
         YADOMS_LOG_CONFIGURE("Websocket");
      }

      void CWebsocketListener::onPing(const WebSocket& socket, const oatpp::String& message)
      {
         YADOMS_LOG(debug) << "onPing";
         socket.sendPong(message);
      }

      void CWebsocketListener::onPong(const WebSocket& socket, const oatpp::String& message)
      {
         YADOMS_LOG(debug) << "onPong";
      }

      void CWebsocketListener::onClose(const WebSocket& socket, v_uint16 code, const oatpp::String& message)
      {
         YADOMS_LOG(debug) << "onClose, code=" << code;
      }

      void CWebsocketListener::readMessage(const WebSocket& socket,
                                           v_uint8 opcode,
                                           p_char8 data,
                                           oatpp::v_io_size size)
      {
         if (size > 0)
         {
            // message frame received

            m_buffer.writeSimple(data, static_cast<v_buff_size>(size));
            return;
         }

         if (size == 0)
         {
            // message transfer finished

            const auto wholeMessage = m_buffer.toString();
            m_buffer.clear();

            YADOMS_LOG(trace) << "onMessage " << wholeMessage->c_str();

            /* Send message in reply */
            socket.sendOneFrameText("Hello from oatpp!: " + wholeMessage);
         }
      }
   } //namespace oatppServer
} //namespace web

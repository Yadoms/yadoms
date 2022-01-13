#pragma once
#include <oatpp-websocket/ConnectionHandler.hpp>

namespace web
{
   namespace oatppServer
   {
      class CWebsocketListener : public oatpp::websocket::WebSocket::Listener
      {
      public:
         CWebsocketListener();

         void onPing(const WebSocket& socket,
                     const oatpp::String& message) override;
         void onPong(const WebSocket& socket,
                     const oatpp::String& message) override;
         void onClose(const WebSocket& socket,
                      v_uint16 code,
                      const oatpp::String& message) override;
         void readMessage(const WebSocket& socket,
                          v_uint8 opcode,
                          p_char8 data,
                          oatpp::v_io_size size) override;

      private:
         std::string processReceivedMessage(const std::string receivedMessage);

         oatpp::data::stream::BufferOutputStream m_messageBuffer;;
      };
   } //namespace oatppServer
} //namespace web

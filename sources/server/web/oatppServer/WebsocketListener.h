#pragma once
#include <oatpp-websocket/ConnectionHandler.hpp>

#include "WebsocketOnNewAcquisitionHandler.h"

namespace web
{
   namespace oatppServer
   {
      class CWebsocketListener final : public oatpp::websocket::WebSocket::Listener
      {
      public:
         CWebsocketListener(boost::shared_ptr<CWebsocketOnNewAcquisitionHandler> acquisitionObserver);
         ~CWebsocketListener() override = default;

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
         static void sendMessage(const std::string& message,
                                 const WebSocket& socket);
         static std::string makeIsAliveReply();
         void processReceivedMessage(const std::string& receivedMessage,
                                     const WebSocket& socket) const;

         oatpp::data::stream::BufferOutputStream m_messageBuffer;
         boost::shared_ptr<CWebsocketOnNewAcquisitionHandler> m_acquisitionObserver;
      };
   } //namespace oatppServer
} //namespace web

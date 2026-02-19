#pragma once
#include <oatpp-websocket/ConnectionHandler.hpp>

#include "shared/event/EventHandler.hpp"

namespace web::oatpp_server
{
   class CWebsocketListener final : public oatpp::websocket::WebSocket::Listener
   {
   public:
      CWebsocketListener(shared::event::CEventHandler& eventHandler,
                         int onPongEventId,
                         int onPingEventId,
                         int onReceiveEventId);
      ~CWebsocketListener() override = default;

      CWebsocketListener(const CWebsocketListener&) = delete;
      CWebsocketListener(CWebsocketListener&&) = delete;
      CWebsocketListener operator=(const CWebsocketListener&) = delete;
      CWebsocketListener operator=(CWebsocketListener&&) = delete;

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
      oatpp::data::stream::BufferOutputStream m_messageBuffer;
      shared::event::CEventHandler& m_eventHandler; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
      int m_onPongEventId;
      int m_onPingEventId;
      int m_onReceiveEventId;
   };
}

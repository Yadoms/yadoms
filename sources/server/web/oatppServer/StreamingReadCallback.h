#pragma once
#include <oatpp/web/server/HttpRequestHandler.hpp>

#include <shared/event/EventHandler.hpp>

namespace web
{
   namespace oatppServer
   {
      class CSseClientConnection final //TODO dépalcer
      {
      public:
         CSseClientConnection(boost::shared_ptr<shared::event::CEventHandler> eventHandler,
                              int streamingEventId);

         ~CSseClientConnection();

         oatpp::String handle() const;

      private:
         static unsigned int m_connectionsCount;

         boost::shared_ptr<shared::event::CEventHandler> m_eventHandler;
         const int m_streamingEventId;
      };

      class CStreamingReadCallback final : public oatpp::data::stream::ReadCallback
      {
      public:
         CStreamingReadCallback(boost::shared_ptr<shared::event::CEventHandler> eventHandler,
                                int streamingEventId);

         ~CStreamingReadCallback() override = default;

         oatpp::v_io_size read(void* buffer, v_buff_size count, oatpp::async::Action& action) override;

      private:
         oatpp::String m_currentMessage;
         oatpp::data::buffer::InlineWriteData m_inlineData;

         boost::shared_ptr<CSseClientConnection> m_clientConnection;
      };
   } //namespace oatppServer
} //namespace web

#pragma once
#include <oatpp/web/server/HttpRequestHandler.hpp>

#include <shared/event/EventHandler.hpp>

namespace web
{
   namespace oatppServer
   {
      class CStreamingReadCallback final : public oatpp::data::stream::ReadCallback
      {
      public:
         CStreamingReadCallback(boost::shared_ptr<shared::event::CEventHandler> eventHandler,
                                int streamingEventId);

         ~CStreamingReadCallback() override;

         oatpp::v_io_size read(void* buffer, v_buff_size count, oatpp::async::Action& action) override;

      private:
         oatpp::String waitEvent() const;

         oatpp::String m_currentMessage;
         oatpp::data::buffer::InlineWriteData m_inlineData;

         boost::shared_ptr<shared::event::CEventHandler> m_eventHandler;
         const int m_streamingEventId;

         static unsigned int m_connectionsCount;
      };
   } //namespace oatppServer
} //namespace web

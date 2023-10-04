#pragma once
#include <oatpp/web/server/HttpRequestHandler.hpp>

#include "web/rest/IRequest.h"
#include "web/rest/IAnswer.h"
#include "IAuthentication.h"
#include <shared/event/EventHandler.hpp>

namespace web
{
   namespace oatppServer
   {
      class CStreamingReadCallback final : public oatpp::data::stream::ReadCallback //TODO déplacer
      {
      public:
         explicit CStreamingReadCallback(boost::shared_ptr<shared::event::CEventHandler> eventHandler,
                                         int streamingEventId)
            : m_eventHandler(std::move(eventHandler)),
              m_streamingEventId(streamingEventId)
         {
         }

         ~CStreamingReadCallback() override //TODO conserver ?
         {
         }

         oatpp::String waitEvent() const
         {
            if (m_eventHandler->waitForEvents() == m_streamingEventId)
               return "event: hello !\n"
                  "data: <some data>\n\n";
            return {};
         }

         oatpp::v_io_size read(void* buffer, v_buff_size count, oatpp::async::Action& action) override
         {
            if (m_inlineData.bytesLeft == 0)
            {
               m_currentMessage = waitEvent();
               if (m_currentMessage) m_inlineData.set(m_currentMessage->data(), m_currentMessage->size());
            }

            v_buff_size desiredToRead = m_inlineData.bytesLeft;
            if (desiredToRead > 0)
            {
               if (desiredToRead > count) desiredToRead = count;
               std::memcpy(buffer, m_inlineData.currBufferPtr, desiredToRead);
               m_inlineData.inc(desiredToRead);
               return desiredToRead;
            }

            return 0;
         }

      private:
         oatpp::String m_currentMessage;
         oatpp::data::buffer::InlineWriteData m_inlineData;

         boost::shared_ptr<shared::event::CEventHandler> m_eventHandler;
         const int m_streamingEventId;
      };

      class CRestRequestHandler final : public oatpp::web::server::HttpRequestHandler
      {
      public:
         explicit CRestRequestHandler(std::function<boost::shared_ptr<rest::IAnswer>(boost::shared_ptr<rest::IRequest>)> handler,
                                      boost::shared_ptr<IAuthentication> authentication);
         ~CRestRequestHandler() override = default;

         // oatpp::web::server::HttpRequestHandler Implementation
         std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override;
         // [END] oatpp::web::server::HttpRequestHandler Implementation

      private:
         static const oatpp::web::protocol::http::Status& toStatusCode(const shared::http::ECodes& error);

         std::function<boost::shared_ptr<rest::IAnswer>(boost::shared_ptr<rest::IRequest>)> m_handler;
         const boost::shared_ptr<IAuthentication> m_authentication;
      };
   } //namespace oatppServer
} //namespace web

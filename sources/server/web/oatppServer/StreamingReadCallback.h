#pragma once
#include <oatpp/web/server/HttpRequestHandler.hpp>

#include "shared/http/ssdp/Client.h"
#include "web/rest/service/ISseConnectionHandler.h"

namespace web
{
   namespace oatppServer
   {
      class CStreamingReadCallback final : public oatpp::data::stream::ReadCallback
      {
      public:
         explicit CStreamingReadCallback(std::shared_ptr<rest::service::ISseConnectionHandler> connectionHandler);

         ~CStreamingReadCallback() override;

         oatpp::v_io_size read(void* buffer,
                               v_buff_size count,
                               oatpp::async::Action& action) override;

      private:
         static unsigned int m_connectionsCount;

         oatpp::String m_currentMessage;
         oatpp::data::buffer::InlineWriteData m_inlineData;

         std::shared_ptr<rest::service::ISseConnectionHandler> m_connectionHandler;
      };
   } //namespace oatppServer
} //namespace web

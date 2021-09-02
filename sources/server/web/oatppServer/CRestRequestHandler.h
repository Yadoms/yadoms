#pragma once
#include <oatpp/web/server/HttpRequestHandler.hpp>

#include "web/rest/RequestV2.h"
#include "web/rest/ResultV2.h"

namespace web
{
   namespace oatppServer
   {
      class CRestRequestHandler final : public oatpp::web::server::HttpRequestHandler
      {
      public:
         explicit CRestRequestHandler(std::function<boost::shared_ptr<rest::IRestAnswer>(boost::shared_ptr<rest::IRestRequest>)> handler);
         ~CRestRequestHandler() override = default;

         // oatpp::web::server::HttpRequestHandler Implementation
         std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override;
         // [END] oatpp::web::server::HttpRequestHandler Implementation

      private:
         static const oatpp::web::protocol::http::Status& toStatusCode(const shared::http::ECodes& error);

         std::function<boost::shared_ptr<rest::IRestAnswer>(boost::shared_ptr<rest::IRestRequest>)> m_handler;
      };
   } //namespace oatppServer
} //namespace web

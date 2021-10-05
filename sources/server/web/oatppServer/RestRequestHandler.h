#pragma once
#include <oatpp/web/server/HttpRequestHandler.hpp>

#include "web/rest/IRequest.h"
#include "web/rest/IAnswer.h"
#include "IAuthentication.h"

namespace web
{
   namespace oatppServer
   {
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

#pragma once
#include <oatpp/web/server/HttpRequestHandler.hpp>
#include <oatpp/web/server/handler/ErrorHandler.hpp>

namespace web::oatpp_server
{
   //
   //\brief The default error handler
   //
   class CErrorHandler final : public oatpp::web::server::handler::DefaultErrorHandler
   {
   public:
      ~CErrorHandler() override = default;

      CErrorHandler() = default;
      CErrorHandler(const CErrorHandler&) = delete;
      CErrorHandler& operator=(const CErrorHandler&) = delete;
      CErrorHandler(CErrorHandler&&) = delete;
      CErrorHandler& operator=(CErrorHandler&&) = delete;

      std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> renderError(const HttpServerErrorStacktrace& stacktrace) override;
   };
}

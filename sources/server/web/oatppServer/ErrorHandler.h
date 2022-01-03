#pragma once
#include <oatpp/web/server/HttpRequestHandler.hpp>
#include <oatpp/web/server/handler/ErrorHandler.hpp>

namespace web
{
   namespace oatppServer
   {
      //
      //\brief The default error handler (written from oatpp::web::server::handler::DefaultErrorHandler)
      //
      class CErrorHandler final : public oatpp::base::Countable, public oatpp::web::server::handler::ErrorHandler
      {
      public:
         ~CErrorHandler() override = default;

         std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> handleError(const oatpp::web::protocol::http::Status& status,
                                                                                     const oatpp::String& message,
                                                                                     const Headers& headers) override;
      };
   } //namespace oatppServer
} //namespace web

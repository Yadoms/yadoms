#pragma once

namespace web
{
   namespace oatppServer
   {
      //-------------------------------------
      ///\brief Interface for authentication
      //-------------------------------------
      class IAuthentication
      {
      public:
         virtual ~IAuthentication() = default;

         //-------------------------------------
         ///\brief Check authentication for a request
         ///\param [in]    request       The request to check authentication
         ///\throw oatpp::web::protocol::http::HttpError if authentication fails
         //-------------------------------------   
         virtual void authenticate(const std::shared_ptr<oatpp::web::server::HttpRequestHandler::IncomingRequest>& request) const = 0;
      };
   } // namespace oatppServer
} //namespace web

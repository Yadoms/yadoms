#include "stdafx.h"

#include "ErrorHandler.h"
#include <oatpp/web/protocol/http/outgoing/BufferBody.hpp>

namespace web
{
   namespace oatppServer
   {
      std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> CErrorHandler::handleError(
         const oatpp::web::protocol::http::Status& status,
         const oatpp::String& message,
         const oatpp::web::protocol::http::Headers& headers)
      {
         static oatpp::String serverHeader(("yadoms/" + std::string(YADOMS_VERSION)).c_str());

         oatpp::data::stream::BufferOutputStream stream;
         stream << "server=" << serverHeader << "\n";
         stream << "code=" << status.code << "\n";
         stream << "description=" << status.description << "\n";
         stream << "message=" << message << "\n";
         auto response = oatpp::web::protocol::http::outgoing::Response::createShared
            (status, oatpp::web::protocol::http::outgoing::BufferBody::createShared(stream.toString()));

         response->putHeader(oatpp::web::protocol::http::Header::SERVER, serverHeader);
         response->putHeader(oatpp::web::protocol::http::Header::CONNECTION, oatpp::web::protocol::http::Header::Value::CONNECTION_CLOSE);

         for (const auto& pair : headers.getAll())
         {
            response->putHeader_Unsafe(pair.first, pair.second);
         }

         return response;
      }
   } //namespace oatppServer
} //namespace web

#include "stdafx.h"

#include "ErrorHandler.h"
#include <oatpp/web/protocol/http/outgoing/BufferBody.hpp>

using namespace web::oatpp_server;

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> CErrorHandler::renderError(
   const HttpServerErrorStacktrace& stacktrace)
{
   static oatpp::String serverHeader(("yadoms/" + std::string(YADOMS_VERSION)).c_str());

   oatpp::data::stream::BufferOutputStream stream;
   stream << "server=" << serverHeader << "\n";
   stream << "code=" << stacktrace.status.code << "\n";
   stream << "description=" << stacktrace.status.description << "\n";
   stream << "stacktrace:\n";

   for (auto& msg : stacktrace.stack)
      stream << "  - " << msg << "\n";

   auto response = oatpp::web::protocol::http::outgoing::Response::createShared
      (stacktrace.status, oatpp::web::protocol::http::outgoing::BufferBody::createShared(stream.toString()));

   response->putHeaderIfNotExists(oatpp::web::protocol::http::Header::SERVER, serverHeader);
   response->putHeaderIfNotExists(oatpp::web::protocol::http::Header::CONNECTION, oatpp::web::protocol::http::Header::Value::CONNECTION_CLOSE);

   for (const auto& [key, value] : stacktrace.headers.getAll())
      response->putHeader_Unsafe(key, value);

   return response;
}

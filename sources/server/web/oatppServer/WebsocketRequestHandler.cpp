#include "stdafx.h"
#include "WebsocketRequestHandler.h"
#include <oatpp-websocket/Handshaker.hpp>

using namespace web::oatpp_server;

CWebsocketRequestHandler::CWebsocketRequestHandler(std::shared_ptr<oatpp::websocket::ConnectionHandler> websocketConnectionHandler)
   : m_websocketConnectionHandler(std::move(websocketConnectionHandler))
{
}

std::shared_ptr<oatpp::web::server::HttpRequestHandler::OutgoingResponse> CWebsocketRequestHandler::handle(
   const std::shared_ptr<IncomingRequest>& request)
{
   return oatpp::websocket::Handshaker::serversideHandshake(request->getHeaders(),
                                                            m_websocketConnectionHandler);
}

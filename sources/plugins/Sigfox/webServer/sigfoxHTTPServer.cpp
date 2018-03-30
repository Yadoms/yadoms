#include "stdafx.h"
#include "sigfoxHTTPServer.h"
#include <Poco/Net/ServerSocket.h>
#include "sigfoxRequestHandlerFactory.h"
#include <shared/Log.h>

CSigfoxHTTPServer::CSigfoxHTTPServer(shared::event::CEventHandler& receiveDataEventHandler,
                                     int receiveDataEventId,
                                     const unsigned short port)
{
   m_embeddedWebServer = boost::make_shared<Poco::Net::HTTPServer>(new CSigfoxRequestHandlerFactory(receiveDataEventHandler,
                                                                                                    receiveDataEventId),
                                                                   Poco::Net::ServerSocket(port),
                                                                   new Poco::Net::HTTPServerParams);
}

CSigfoxHTTPServer::~CSigfoxHTTPServer()
{}

void CSigfoxHTTPServer::start()
{
   YADOMS_LOG(trace) << "web server started at port " << m_embeddedWebServer->port();
   m_embeddedWebServer->start();
}

void CSigfoxHTTPServer::stop()
{
   YADOMS_LOG(trace) << "web server stopped";
   m_embeddedWebServer->stop();
}
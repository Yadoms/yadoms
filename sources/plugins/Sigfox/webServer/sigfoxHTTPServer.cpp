#include "stdafx.h"
#include "sigfoxHTTPServer.h"
#include <Poco/Net/ServerSocket.h>
#include "sigfoxRequestHandlerFactory.h"
#include <shared/Log.h>

CSigfoxHTTPServer::CSigfoxHTTPServer(const unsigned short port)
{
   m_embeddedWebServer = boost::make_shared<Poco::Net::HTTPServer>(new CSigfoxRequestHandlerFactory(),
                                                                   Poco::Net::ServerSocket(port),
                                                                   new Poco::Net::HTTPServerParams);
}

CSigfoxHTTPServer::~CSigfoxHTTPServer()
{}

void CSigfoxHTTPServer::start()
{
   m_embeddedWebServer->start();
}

void CSigfoxHTTPServer::stop()
{
   m_embeddedWebServer->stop();
}
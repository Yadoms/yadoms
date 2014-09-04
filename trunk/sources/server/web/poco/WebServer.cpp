#include "stdafx.h"
#include "WebServer.h"
#include <shared/Log.h>

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/WebSocket.h>
#include <Poco/Net/NetException.h>
#include "MimeType.h"



namespace web { namespace poco {


   CWebServer::CWebServer(const std::string & address, const std::string & port, const std::string & doc_root, const std::string & restKeywordBase, const std::string & webSocketKeywordBase)
      :m_httpRequestHandlerFactory(new CHttpRequestHandlerFactory())
   {
      //configure the factory
      m_httpRequestHandlerFactory->websiteHandlerConfigure(doc_root);
      m_httpRequestHandlerFactory->restHandlerConfigure(restKeywordBase);
      m_httpRequestHandlerFactory->webSocketConfigure(webSocketKeywordBase);

      // set-up a HTTPServer instance
      if (address == "0.0.0.0" || address.empty())
      {
         //in case of "0.0.0.0" or empty , then do not use it, just use port, listen on all interfaces
         Poco::Net::ServerSocket svs(boost::lexical_cast<unsigned short>(port));
         m_embeddedWebServer.reset(new Poco::Net::HTTPServer(m_httpRequestHandlerFactory, svs, new Poco::Net::HTTPServerParams));
      }
      else
      {
         //if address is specified, try to use it
         Poco::Net::SocketAddress sa(address, boost::lexical_cast<unsigned short>(port));
         Poco::Net::ServerSocket svs(sa);
         m_embeddedWebServer.reset(new Poco::Net::HTTPServer(m_httpRequestHandlerFactory, svs, new Poco::Net::HTTPServerParams));
      }
      
   }

   CWebServer::~CWebServer()
   {
      stop();
   }


   // IWebServer implementation
   void CWebServer::start()
   {
      // start the HTTPServer
      m_embeddedWebServer->start();
   }

   void CWebServer::stop()
   {
      if(m_embeddedWebServer.get())
         m_embeddedWebServer->stop();
   }

   IWebServerConfigurator* CWebServer::getConfigurator()
   {
      return m_httpRequestHandlerFactory.get();
   }




} //namespace poco
} //namespace web


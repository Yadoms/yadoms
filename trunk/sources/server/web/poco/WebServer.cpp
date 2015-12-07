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
#include <shared/ServiceLocator.h>
#include "server/IRunningInformation.h"


namespace web { namespace poco {


   CWebServer::CWebServer(const std::string & address, const std::string & port, const std::string & doc_root, const std::string & restKeywordBase, const std::string & webSocketKeywordBase)
      :m_httpRequestHandlerFactory(new CHttpRequestHandlerFactory())
   {
      //configure the factory
      m_httpRequestHandlerFactory->websiteHandlerConfigure(doc_root);
      m_httpRequestHandlerFactory->restHandlerConfigure(restKeywordBase);
      m_httpRequestHandlerFactory->webSocketConfigure(webSocketKeywordBase);

      //setup HTTPServer Params (define name and version; to match common http server configuration)
      Poco::Net::HTTPServerParams * serverParams = new Poco::Net::HTTPServerParams();
      serverParams->setServerName("Yadoms");
      boost::shared_ptr<IRunningInformation> runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>());
      serverParams->setSoftwareVersion(runningInformation->getSoftwareVersion().getVersion().toString());
      serverParams->setKeepAlive(false); //this line fix global catch exception on multiple browser refresh

      // set-up a HTTPServer instance
      if (address == "0.0.0.0" || address.empty())
      {
         //in case of "0.0.0.0" or empty , then do not use it, just use port, listen on all interfaces
         Poco::Net::ServerSocket svs(boost::lexical_cast<unsigned short>(port));
         bool a = false;
         int b = 0;
         svs.getLinger(a, b);
         YADOMS_LOG(information) << "getLinger on_off: " << a << " time : " << b;
         svs.setLinger(true, 2);
         m_embeddedWebServer = boost::make_shared<Poco::Net::HTTPServer>(m_httpRequestHandlerFactory, svs, serverParams);
      }
      else
      {
         //if address is specified, try to use it
         Poco::Net::SocketAddress sa(address, boost::lexical_cast<unsigned short>(port));
         Poco::Net::ServerSocket svs(sa);
         m_embeddedWebServer = boost::make_shared<Poco::Net::HTTPServer>(m_httpRequestHandlerFactory, svs, serverParams);
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
         m_embeddedWebServer->stopAll(true);
   }

   IWebServerConfigurator* CWebServer::getConfigurator()
   {
      return m_httpRequestHandlerFactory.get();
   }




} //namespace poco
} //namespace web


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
#include <Poco/Net/SecureServerSocket.h>
#include <Poco/Net/WebSocket.h>
#include <Poco/Net/NetException.h>
#include "MimeType.h"
#include <shared/ServiceLocator.h>
#include "server/IRunningInformation.h"


namespace web { namespace poco {


   CWebServer::CWebServer(const std::string & address, const bool useSSL, const unsigned short port, const unsigned short securedPort, const std::string & doc_root, const std::string & restKeywordBase, const std::string & webSocketKeywordBase)
      :m_httpRequestHandlerFactory(new CHttpRequestHandlerFactory())
   {
      //configure the factory
      m_httpRequestHandlerFactory->websiteHandlerConfigure(doc_root);
      m_httpRequestHandlerFactory->restHandlerConfigure(restKeywordBase);
      m_httpRequestHandlerFactory->webSocketConfigure(webSocketKeywordBase);

      //setup HTTPServer Params (define name and version; to match common http server configuration)
      Poco::Net::HTTPServerParams::Ptr serverParams(new Poco::Net::HTTPServerParams());
      serverParams->setServerName("Yadoms");
      boost::shared_ptr<IRunningInformation> runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>());
      serverParams->setSoftwareVersion(runningInformation->getSoftwareVersion().getVersion().toString());
      serverParams->setKeepAlive(false); //this line fix global catch exception on multiple browser refresh

      //in case of "0.0.0.0" or empty , then do not use it, just use port, listen on all interfaces
      Poco::Net::IPAddress ipAddress;
      if (address == "0.0.0.0" || address.empty())
      {
         ipAddress = Poco::Net::IPAddress::wildcard();
      }
      else
      {
         if (!Poco::Net::IPAddress::tryParse(address, ipAddress))
         {
            //invalid IP provided
            YADOMS_LOG(warning) << "*******************************************************";
            YADOMS_LOG(warning) << "Invalid IP address provided in web server configuration";
            YADOMS_LOG(warning) << "Listening on any ip address";
            YADOMS_LOG(warning) << "*******************************************************";
            ipAddress = Poco::Net::IPAddress::wildcard();
         }
      }

      // set-up a HTTPServer instance
      //in case of "0.0.0.0" or empty , then do not use it, just use port, listen on all interfaces
      Poco::Net::SocketAddress sa(ipAddress, port);
      Poco::Net::ServerSocket svs(sa);
      if (useSSL)
      {
         bool fail = false;

         try {
            Poco::Net::SocketAddress sa_secure(address, securedPort);
            svs = Poco::Net::SecureServerSocket(sa_secure);
         }
         catch (Poco::Exception & ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to configure HTTPS: " << ex.message();
            fail = true;
         }
         catch (std::exception & ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to configure HTTPS: " << ex.what();
            fail = true;
         }
         catch (...)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to configure HTTPS";
            fail = true;
         }
         if (fail)
         {
            //we activate the default classic web server
            YADOMS_LOG(warning) << "**********************************************************";
            YADOMS_LOG(warning) << "Using HTTP instead of HTTPS due to malformed configuration";
            YADOMS_LOG(warning) << "**********************************************************";
            Poco::Net::SocketAddress sa_http(address, port);
            svs = Poco::Net::ServerSocket(sa_http);
         }
      }

      bool a = false;
      int b = 0;
      svs.getLinger(a, b);
      svs.setLinger(true, 2);
      m_embeddedWebServer = boost::make_shared<Poco::Net::HTTPServer>(m_httpRequestHandlerFactory, svs, serverParams);
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


#include "stdafx.h"
#include "WebServer.h"
#include <shared/Log.h>

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SecureServerSocket.h>
#include <shared/ServiceLocator.h>
#include "IRunningInformation.h"


namespace web
{
   namespace poco
   {
      CWebServer::CWebServer(const std::string& address,
                             bool useSsl,
                             unsigned short port,
                             unsigned short securedPort,
                             const std::string& docRoot,
                             const std::string& restKeywordBase,
                             const std::string& webSocketKeywordBase,
                             bool allowExternalAccess)
         : m_httpRequestHandlerFactory(new CHttpRequestHandlerFactory())
      {
         //configure the factory
         m_httpRequestHandlerFactory->websiteHandlerConfigure(docRoot);
         m_httpRequestHandlerFactory->restHandlerConfigure(restKeywordBase);
         m_httpRequestHandlerFactory->webSocketConfigure(webSocketKeywordBase);
         m_httpRequestHandlerFactory->allowExternalAccess(allowExternalAccess);

         //setup HTTPServer Params (define name and version; to match common http server configuration)
         Poco::Net::HTTPServerParams::Ptr serverParams(new Poco::Net::HTTPServerParams());
         serverParams->setServerName("Yadoms");
         const auto runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>());
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
         if (useSsl)
         {
            auto fail = false;

            try
            {
               const Poco::Net::SocketAddress saSecure(address, securedPort);
               svs = Poco::Net::SecureServerSocket(saSecure);
            }
            catch (Poco::Exception& ex)
            {
               //fail to download package
               YADOMS_LOG(error) << "Fail to configure HTTPS: " << ex.message();
               fail = true;
            }
            catch (std::exception& ex)
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
               const Poco::Net::SocketAddress saHttp(address, port);
               svs = Poco::Net::ServerSocket(saHttp);
            }
         }

         auto a = false;
         auto b = 0;
         svs.getLinger(a, b);
         svs.setLinger(true, 2);
         m_embeddedWebServer = boost::make_shared<Poco::Net::HTTPServer>(m_httpRequestHandlerFactory, svs, serverParams);
      }

      CWebServer::~CWebServer()
      {
         stop();
      }


      // IWebServer implementation
      void CWebServer::start() const
      {
         // start the HTTPServer
         m_embeddedWebServer->start();
      }

      void CWebServer::stop() const
      {
         if (m_embeddedWebServer.get())
            m_embeddedWebServer->stopAll(true);
      }

      IWebServerConfigurator* CWebServer::getConfigurator()
      {
         return m_httpRequestHandlerFactory.get();
      }

      void CWebServer::websiteHandlerAddAlias(const std::string& alias,
                                              const std::string& path)
      {
         m_httpRequestHandlerFactory->websiteHandlerAddAlias(alias,
                                                             path);
      }
   } //namespace poco
} //namespace web

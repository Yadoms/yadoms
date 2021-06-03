#include "WebServer.h"
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <shared/Log.h>
#include <shared/ServiceLocator.h>
#include "IRunningInformation.h"
#include "stdafx.h"

namespace web
{
   namespace oatppServer
   {
      CWebServer::CWebServer(const std::string& address,
                             bool useSSL,
                             unsigned short port,
                             unsigned short securedPort,
                             const std::string& docRoot,
                             const std::string& restKeywordBase,
                             const std::string& webSocketKeywordBase,
                             bool allowExternalAccess)
      //TODO: m_httpRequestHandlerFactory(new CHttpRequestHandlerFactory())
      {
         oatpp::base::Environment::init();


         // Create ConnectionProvider component which listens on the port
         OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([&address, &port]
               {
                  return oatpp::network::tcp::server::ConnectionProvider::createShared({
                     (address.empty() ? "0.0.0.0" : address).c_str(),
                     port,
                     oatpp::network::Address::IP_4
                  });
               }());

         // Create Router component
         OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([]
               {
                  return oatpp::web::server::HttpRouter::createShared();
               }());

         // Create ConnectionHandler component which uses Router component to route requests
         OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([]
               {
                  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
                  OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper); // get ObjectMapper component

                  auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);
                  //TODO                  connectionHandler->setErrorHandler(std::make_shared<ErrorHandler>(objectMapper));
                  return connectionHandler;
               }());


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
         if (useSSL)
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
         CWebServer::stop();
         oatpp::base::Environment::destroy();
      }


      // IWebServer implementation
      void CWebServer::start()
      {
         // start the HTTPServer
         m_embeddedWebServer->start();
      }

      void CWebServer::stop()
      {
         if (m_embeddedWebServer.get())
            m_embeddedWebServer->stopAll(true);
      }

      IWebServerConfigurator* CWebServer::getConfigurator()
      {
         return m_httpRequestHandlerFactory.get();
      }
   } //namespace oatppServer
} //namespace web

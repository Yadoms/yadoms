#include "stdafx.h"
#include "WebServer.h"
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <shared/Log.h>

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
         : m_httpRequestHandlerFactory(std::make_shared<CHttpRequestHandlerFactory>())
      {
         oatpp::base::Environment::init();

         // Create Router component
         auto httpRouter = oatpp::web::server::HttpRouter::createShared();
         refreshRoutes(httpRouter);

         m_httpConnectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(httpRouter);

         m_tcpConnectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared(
            {
               (address.empty() ? "0.0.0.0" : address).c_str(),
               port,
               oatpp::network::Address::IP_4
            });

         m_server = std::make_shared<oatpp::network::Server>(m_tcpConnectionProvider,
                                                             m_httpConnectionHandler);

         // Configure the factory
         m_httpRequestHandlerFactory->websiteHandlerConfigure(docRoot);
         m_httpRequestHandlerFactory->restHandlerConfigure(restKeywordBase);
         m_httpRequestHandlerFactory->webSocketConfigure(webSocketKeywordBase);
         m_httpRequestHandlerFactory->allowExternalAccess(allowExternalAccess);

         ////setup HTTPServer Params (define name and version; to match common http server configuration)
         //Poco::Net::HTTPServerParams::Ptr serverParams(new Poco::Net::HTTPServerParams());
         //serverParams->setServerName("Yadoms");
         //const auto runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>());
         //serverParams->setSoftwareVersion(runningInformation->getSoftwareVersion().getVersion().toString());
         //serverParams->setKeepAlive(false); //this line fix global catch exception on multiple browser refresh

         ////in case of "0.0.0.0" or empty , then do not use it, just use port, listen on all interfaces
         //Poco::Net::IPAddress ipAddress;
         //if (address == "0.0.0.0" || address.empty())
         //{
         //   ipAddress = Poco::Net::IPAddress::wildcard();
         //}
         //else
         //{
         //   if (!Poco::Net::IPAddress::tryParse(address, ipAddress))
         //   {
         //      //invalid IP provided
         //      YADOMS_LOG(warning) << "*******************************************************";
         //      YADOMS_LOG(warning) << "Invalid IP address provided in web server configuration";
         //      YADOMS_LOG(warning) << "Listening on any ip address";
         //      YADOMS_LOG(warning) << "*******************************************************";
         //      ipAddress = Poco::Net::IPAddress::wildcard();
         //   }
         //}

         //// set-up a HTTPServer instance
         ////in case of "0.0.0.0" or empty , then do not use it, just use port, listen on all interfaces
         //Poco::Net::SocketAddress sa(ipAddress, port);
         //Poco::Net::ServerSocket svs(sa);
         //if (useSSL)
         //{
         //   auto fail = false;

         //   try
         //   {
         //      const Poco::Net::SocketAddress saSecure(address, securedPort);
         //      svs = Poco::Net::SecureServerSocket(saSecure);
         //   }
         //   catch (Poco::Exception& ex)
         //   {
         //      //fail to download package
         //      YADOMS_LOG(error) << "Fail to configure HTTPS: " << ex.message();
         //      fail = true;
         //   }
         //   catch (std::exception& ex)
         //   {
         //      //fail to download package
         //      YADOMS_LOG(error) << "Fail to configure HTTPS: " << ex.what();
         //      fail = true;
         //   }
         //   catch (...)
         //   {
         //      //fail to download package
         //      YADOMS_LOG(error) << "Fail to configure HTTPS";
         //      fail = true;
         //   }
         //   if (fail)
         //   {
         //      //we activate the default classic web server
         //      YADOMS_LOG(warning) << "**********************************************************";
         //      YADOMS_LOG(warning) << "Using HTTP instead of HTTPS due to malformed configuration";
         //      YADOMS_LOG(warning) << "**********************************************************";
         //      const Poco::Net::SocketAddress saHttp(address, port);
         //      svs = Poco::Net::ServerSocket(saHttp);
         //   }
         //}

         //auto a = false;
         //auto b = 0;
         //svs.getLinger(a, b);
         //svs.setLinger(true, 2);
         //m_embeddedWebServer = boost::make_shared<Poco::Net::HTTPServer>(m_httpRequestHandlerFactory, svs, serverParams);
      }

      CWebServer::~CWebServer()
      {
         CWebServer::stop();
         oatpp::base::Environment::destroy();
      }


      // IWebServer implementation
      void CWebServer::start()
      {
         m_serverThread = std::thread([this]
         {
            m_server->run();
         });
      }

      void CWebServer::stop()
      {
         // First, stop the ServerConnectionProvider so we don't accept any new connections
         m_tcpConnectionProvider->stop();

         // Stop server if still running
         if (m_server->getStatus() == oatpp::network::Server::STATUS_RUNNING)
            m_server->stop();

         // Finally, stop the ConnectionHandler and wait until all running connections are closed
         m_httpConnectionHandler->stop();

         // Wait for thread stopped
         if (m_serverThread.joinable())
            m_serverThread.join();
      }

      IWebServerConfigurator* CWebServer::getConfigurator()
      {
         return m_httpRequestHandlerFactory.get();
      }

      void CWebServer::refreshRoutes(const std::shared_ptr<oatpp::web::server::HttpRouter>& httpRouter)
      {
         const auto pagesFiles = std::make_shared<CHttpPages>("Yadoms client", "www");
         httpRouter->route("GET", "/", pagesFiles);

         routeAllFiles("www", httpRouter, pagesFiles);
      }

      void CWebServer::routeAllFiles(const boost::filesystem::path& rootFolder,
                                     const std::shared_ptr<oatpp::web::server::HttpRouter>& httpRouter,
                                     const std::shared_ptr<CHttpPages>& pagesFiles)
      {
         for (boost::filesystem::recursive_directory_iterator end, dir(rootFolder); dir != end; ++dir)
         {
            if (dir->status().type() == boost::filesystem::directory_file)
               continue;

            //TODO faire mieux en utilisant les paths
            auto file = dir->path().string();
            auto page = file.substr(rootFolder.size());
#if _WIN32
            std::replace(page.begin(), page.end(), '\\', '/');
#endif
            httpRouter->route("GET", page.c_str(), pagesFiles);
         }
      }
   } //namespace oatppServer
} //namespace web

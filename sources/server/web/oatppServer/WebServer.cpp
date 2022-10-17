#include "stdafx.h"
#include "WebServer.h"

#include <utility>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <oatpp-websocket/Handshaker.hpp>
#include <oatpp-openssl/server/ConnectionProvider.hpp>
#include <oatpp-openssl/Config.hpp>
#include "ErrorHandler.h"
#include "RestRequestHandler.h"
#include "WebSocketConnection.h"
#include "WebsocketRequestHandler.h"

namespace web
{
   namespace oatppServer
   {
      CWebServer::CWebServer(const std::string& address,
                             unsigned short port,
                             bool useHttps,
                             unsigned short httpsPort,
                             const boost::filesystem::path& docRoot,
                             const boost::filesystem::path& httpsLocalCertificateFile,
                             const boost::filesystem::path& httpsPrivateKeyFile,
                             const std::string& restKeywordBase,
                             boost::shared_ptr<std::vector<boost::shared_ptr<rest::service::IRestService>>> restServices,
                             const std::string& webSocketKeywordBase,
                             const boost::shared_ptr<IAuthentication>& authentication)
         : m_restServices(std::move(restServices))
      {
         oatpp::base::Environment::init();

         // HTTP
         const auto httpRouter = oatpp::web::server::HttpRouter::createShared();
         refreshWebPagesRoutes(httpRouter,
                               docRoot,
                               authentication);
         refreshRestRoutes(httpRouter,
                           restKeywordBase,
                           authentication);

         m_httpConnectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(httpRouter);

         m_httpConnectionHandler->setErrorHandler(std::make_shared<CErrorHandler>());

         if (useHttps)
         {
            // HTTPS
            const auto config = oatpp::openssl::Config::createDefaultServerConfigShared(httpsLocalCertificateFile.string(),
                                                                                        httpsPrivateKeyFile.string());
            m_tcpConnectionProvider = oatpp::openssl::server::ConnectionProvider::createShared(
               config,
               {
                  (address.empty() ? "0.0.0.0" : address).c_str(),
                  httpsPort
               },
               false);
         }
         else
         {
            // HTTP
            m_tcpConnectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared(
               {
                  (address.empty() ? "0.0.0.0" : address).c_str(),
                  port
               });
         }

         m_server = std::make_shared<oatpp::network::Server>(m_tcpConnectionProvider,
                                                             m_httpConnectionHandler);

         // Websocket
         m_websocketConnection = std::make_shared<CWebSocketConnection>();
         m_websocketConnectionHandler = oatpp::websocket::ConnectionHandler::createShared();
         m_websocketConnectionHandler->setSocketInstanceListener(m_websocketConnection);
         httpRouter->route("GET",
                           std::string("/" + webSocketKeywordBase + "/v2").c_str(),
                           std::make_shared<CWebsocketRequestHandler>(m_websocketConnectionHandler));

         start();
      }

      CWebServer::~CWebServer()
      {
         stop();
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
         if (!m_serverThread.joinable())
            return;

         // Close websocket
         m_websocketConnection->close();
         m_websocketConnectionHandler->stop();

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

      void CWebServer::refreshWebPagesRoutes(const std::shared_ptr<oatpp::web::server::HttpRouter>& httpRouter,
                                             const boost::filesystem::path& docRoot,
                                             const boost::shared_ptr<IAuthentication>& authentication)
      {
         const auto pagesFiles = std::make_shared<CHttpPages>(docRoot,
                                                              authentication);
         httpRouter->route("GET", "/", pagesFiles);

         routeAllFiles(docRoot, httpRouter, pagesFiles);
      }

      void CWebServer::routeAllFiles(const boost::filesystem::path& rootFolder,
                                     const std::shared_ptr<oatpp::web::server::HttpRouter>& httpRouter,
                                     const std::shared_ptr<CHttpPages>& pagesFiles)
      {
         for (boost::filesystem::recursive_directory_iterator end, dir(rootFolder); dir != end; ++dir)
         {
            if (dir->status().type() == boost::filesystem::directory_file)
               continue;

            auto relativeUrl = relative(dir->path(), rootFolder).string();
#if _WIN32
            std::replace(relativeUrl.begin(), relativeUrl.end(), '\\', '/');
#endif
            httpRouter->route("GET", relativeUrl.c_str(), pagesFiles);
         }
      }

      void CWebServer::refreshRestRoutes(const std::shared_ptr<oatpp::web::server::HttpRouter>& httpRouter,
                                         const std::string& restKeywordBase,
                                         const boost::shared_ptr<IAuthentication>& authentication) const
      {
         for (const auto& service : *m_restServices)
         {
            for (const auto& endPoint : *service->endPoints())
            {
               static constexpr auto* RestApiVersion = "v2";
               httpRouter->route(ToString(endPoint->verb()).c_str(),
                                 std::string("/" + restKeywordBase + "/" + RestApiVersion + "/" + endPoint->path()).c_str(),
                                 std::make_shared<CRestRequestHandler>(endPoint->handler(),
                                                                       authentication));
            }
         }
      }
   } //namespace oatppServer
} //namespace web

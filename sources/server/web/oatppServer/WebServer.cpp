#include "stdafx.h"
#include "WebServer.h"
#include <utility>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <oatpp-websocket/Handshaker.hpp>
#include "ConnectionProvider.h"
#include "RestRequestHandler.h"
#include "WebsocketInstanceListener.h"

namespace web
{
   namespace oatppServer
   {
      class CHandshakeRequestHandler final : public oatpp::web::server::HttpRequestHandler
      {
      public:
         explicit CHandshakeRequestHandler(std::shared_ptr<oatpp::websocket::ConnectionHandler> websocketConnectionHandler)
            : m_websocketConnectionHandler(std::move(websocketConnectionHandler))
         {
         }

         ~CHandshakeRequestHandler() override = default;

         // oatpp::web::server::HttpRequestHandler Implementation
         std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override
         {
            return oatpp::websocket::Handshaker::serversideHandshake(request->getHeaders(),
                                                                     m_websocketConnectionHandler);
         }

         // [END] oatpp::web::server::HttpRequestHandler Implementation

      private:
         std::shared_ptr<oatpp::websocket::ConnectionHandler> m_websocketConnectionHandler;
      };

      CWebServer::CWebServer(const std::string& address,
                             unsigned short port,
                             bool useSsl,
                             unsigned short securedPort,
                             const boost::filesystem::path& docRoot,
                             const std::string& restKeywordBase,
                             boost::shared_ptr<std::vector<boost::shared_ptr<rest::service::IRestService>>> restServices,
                             const std::string& webSocketKeywordBase,
                             bool allowExternalAccess,
                             boost::shared_ptr<std::map<std::string, boost::filesystem::path>> aliases,
                             const boost::shared_ptr<authentication::IAuthentication>& basicAuthentication)
         : m_aliases(std::move(aliases)),
           m_restServices(std::move(restServices))
      {
         oatpp::base::Environment::init();

         // HTTP
         const auto httpRouter = oatpp::web::server::HttpRouter::createShared();
         refreshWebPagesRoutes(httpRouter,
                               docRoot);
         refreshRestRoutes(httpRouter,
                           restKeywordBase);

         m_httpConnectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(httpRouter);

         m_tcpConnectionProvider = CConnectionProvider::createShared(
            {
               (address.empty() ? "0.0.0.0" : address).c_str(),
               port,
               oatpp::network::Address::IP_4
            });

         m_server = std::make_shared<oatpp::network::Server>(m_tcpConnectionProvider,
                                                             m_httpConnectionHandler);

         // Websocket
         m_websocketConnectionHandler = oatpp::websocket::ConnectionHandler::createShared();
         m_websocketConnectionHandler->setSocketInstanceListener(std::make_shared<CWebsocketInstanceListener>());
         httpRouter->route("GET",
                           std::string("/" + webSocketKeywordBase).c_str(), //TODO besoin préfix "/" ? TODO besoin versionner API ws ?
                           std::make_shared<CHandshakeRequestHandler>(m_websocketConnectionHandler));


         // Configure the factory
         //TODO RAF :
         // - HTTPS
         // - aliases
         // - websockets
         // - allowExternalAccess
         // - basicAuthentication
         // - ajout headers avec infos serveur (nom, version, etc... Voir Poco webserver)

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
                                             const boost::filesystem::path& docRoot)
      {
         const auto pagesFiles = std::make_shared<CHttpPages>(docRoot);
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
                                         const std::string& restKeywordBase) const
      {
         for (const auto& service : *m_restServices)
         {
            for (const auto& endPoint : *service->endPoints())
            {
               static constexpr char* RestApiVersion(R"(v2)");
               httpRouter->route(ToString(endPoint->verb()).c_str(),
                                 std::string("/" + restKeywordBase + "/" + RestApiVersion + "/" + endPoint->path()).c_str(),
                                 std::make_shared<CRestRequestHandler>(endPoint->handler()));
            }
         }
      }
   } //namespace oatppServer
} //namespace web

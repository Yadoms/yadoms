#include "stdafx.h"
#include "WebServer.h"
#include <utility>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <oatpp/web/server/api/Endpoint.hpp>
#include <oatpp-websocket/Handshaker.hpp>
#include <oatpp-swagger/Controller.hpp>
#include "ConnectionProvider.h"
#include "RestRequestHandler.h"
#include "WebSocketConnection.h"

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
                             const boost::shared_ptr<IAuthentication>& authentication)
         : m_aliases(std::move(aliases)),
           m_restServices(std::move(restServices))
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
         m_websocketConnectionHandler->setSocketInstanceListener(std::make_shared<CWebSocketConnection>());
         httpRouter->route("GET",
                           std::string("/" + webSocketKeywordBase).c_str(), //TODO besoin préfix "/" ? TODO besoin versionner API ws ?
                           std::make_shared<CHandshakeRequestHandler>(m_websocketConnectionHandler));


         // Swagger
         // TODO conditionner l'activation par une option dans le .ini
         refreshSwaggerDoc(httpRouter);


         // Configure the factory
         //TODO RAF :
         // - HTTPS
         // - aliases
         // - websockets
         // - allowExternalAccess
         // - ajout headers avec infos serveur (nom, version, etc... Voir Poco webserver) ?

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

      void CWebServer::refreshSwaggerDoc(const std::shared_ptr<oatpp::web::server::HttpRouter>& httpRouter) const
      {
         const auto documentedEndpoints = oatpp::swagger::Controller::Endpoints::createShared();
         for (const auto& service : *m_restServices)
         {
            for (const auto& endPoint : *service->endPoints())
            {
               //TODO déplacer l'interface swagger dans "/rest/v2/"
               if (!endPoint->doc())
                  continue;

               auto documentedEndPoint = toDocumentedEndPoint(endPoint);
               documentedEndpoints->pushBack(documentedEndPoint);
            }
         }

         const auto swaggerController = oatpp::swagger::Controller::createShared(documentedEndpoints);
         swaggerController->addEndpointsToRouter(httpRouter);
      }

      std::shared_ptr<oatpp::web::server::api::Endpoint> CWebServer::toDocumentedEndPoint(
         boost::shared_ptr<rest::service::IRestEndPoint> restEndPoint) const
      {
         return std::make_shared<oatpp::web::server::api::Endpoint>(
            std::make_shared<CRestRequestHandler>(restEndPoint->handler(), nullptr),
            [&restEndPoint]()
            {
               auto info = std::make_shared<oatpp::web::server::api::Endpoint::Info>();

               info->summary = restEndPoint->doc()->summary().c_str();
               info->description = restEndPoint->doc()->description().c_str();

               for(const auto& tag:restEndPoint->doc()->tags())
                  info->addTag(tag.c_str());

               //TODO
               //for (const auto& response : restEndPoint->doc()->responses())
               //   info->addResponse(response.first, response.second);

               return info;
            });
      }
   } //namespace oatppServer
} //namespace web

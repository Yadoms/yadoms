#pragma once
#include <oatpp/network/ConnectionProvider.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <oatpp-websocket/ConnectionHandler.hpp>

#include "IAuthentication.h"
#include "HttpPages.h"
#include "WebSocketConnection.h"
#include "web/IWebServer.h"
#include "web/rest/service/IRestService.h"

namespace web
{
   namespace oatppServer
   {
      //
      //\brief An embedded web server which supports REST api
      //
      class CWebServer final : public IWebServer
      {
      public:
         //
         //\brief                         Create aa embedded w<eb server
         //\param[in] address             IP address.  In general, use  "0.0.0.0"
         //\param[in] port                Port to listen on for browser requests e.g. 8080 
         //\param[in] useHttps            Determine if the server uses HTTPS
         //\param[in] httpsPort           Port to listen on for HTTPS browser requests e.g. 443
         //\param[in] docRoot             Path to folder containing html e.g. "./"
         //\param[in] restKeywordBase     The string which identifies a rest url ex: /rest/
         //\param[in] restServices        Vector of rest services
         //\param[in] authentication      Authentication handling
         CWebServer(const std::string& address,
                    unsigned short port,
                    bool useHttps,
                    unsigned short httpsPort,
                    const boost::filesystem::path& docRoot,
                    const boost::filesystem::path& httpsLocalCertificateFile,
                    const boost::filesystem::path& httpsPrivateKeyFile,
                    const std::string& restKeywordBase,
                    boost::shared_ptr<std::vector<boost::shared_ptr<rest::service::IRestService>>> restServices,
                    const std::string& webSocketKeywordBase,
                    const boost::shared_ptr<IAuthentication>& authentication);
         ~CWebServer() override;

      private:
         void start();
         void stop();

         static void refreshWebPagesRoutes(const std::shared_ptr<oatpp::web::server::HttpRouter>& httpRouter,
                                           const boost::filesystem::path& docRoot,
                                           const boost::shared_ptr<IAuthentication>& authentication);
         static void routeAllFiles(const boost::filesystem::path& rootFolder,
                                   const std::shared_ptr<oatpp::web::server::HttpRouter>& httpRouter,
                                   const std::shared_ptr<CHttpPages>& pagesFiles);
         void refreshRestRoutes(const std::shared_ptr<oatpp::web::server::HttpRouter>& httpRouter,
                                const std::string& restKeywordBase,
                                const boost::shared_ptr<IAuthentication>& authentication) const;


         std::shared_ptr<oatpp::web::server::HttpConnectionHandler> m_httpConnectionHandler;
         std::shared_ptr<oatpp::network::ServerConnectionProvider> m_tcpConnectionProvider;
         std::shared_ptr<CWebSocketConnection> m_websocketConnection;
         std::shared_ptr<oatpp::websocket::ConnectionHandler> m_websocketConnectionHandler;
         std::shared_ptr<oatpp::network::Server> m_server;
         std::thread m_serverThread;

         boost::shared_ptr<std::vector<boost::shared_ptr<rest::service::IRestService>>> m_restServices;
      };
   } //namespace oatppServer
} //namespace web
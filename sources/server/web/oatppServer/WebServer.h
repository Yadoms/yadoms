#pragma once
#include <oatpp/network/ConnectionProvider.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>

#include "HttpPages.h"
#include "HttpRequestHandlerFactory.h"
#include "web/IWebServer.h"

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
         //\param[in] useSsl              Determine if the server use a SSL certificate
         //\param[in] port                Port to listen on for browser requests e.g. 8080 
         //\param[in] securedPort         Port to listen on for HTTPS browser requests e.g. 443
         //\param[in] docRoot             Path to folder containing html e.g. "./"
         //\param[in] restKeywordBase     The string which identifies a rest url ex: /rest/
         //\param[in] restServices        Vector of rest services
         //\param[in] allowExternalAccess Indicate if external access are allowed (in fact it add CORS headers to answer requests)
         //
         CWebServer(const std::string& address,
                    bool useSsl,
                    unsigned short port,
                    unsigned short securedPort,
                    const std::string& docRoot,
                    const std::string& restKeywordBase,
                    boost::shared_ptr<std::vector<boost::shared_ptr<rest::service::IRestService>>> restServices,
                    const std::string& webSocketKeywordBase,
                    bool allowExternalAccess);
         ~CWebServer() override;

         // IWebServer implementation
         IWebServerConfigurator* getConfigurator() override; //TODO virer
         void websiteHandlerAddAlias(const std::string& alias,
                                     const std::string& path) override;
         // [END] IWebServer implementation

      private:
         void start();
         void stop();

         static void refreshWebPagesRoutes(const std::shared_ptr<oatpp::web::server::HttpRouter>& httpRouter,
                                           const std::string& docRoot);
         static void routeAllFiles(const boost::filesystem::path& rootFolder,
                                   const std::shared_ptr<oatpp::web::server::HttpRouter>& httpRouter,
                                   const std::shared_ptr<CHttpPages>& pagesFiles);
         void refreshRestRoutes(const std::shared_ptr<oatpp::web::server::HttpRouter>& httpRouter,
                                const std::string& restKeywordBase) const;

      private:
         std::shared_ptr<oatpp::web::server::HttpConnectionHandler> m_httpConnectionHandler;
         std::shared_ptr<oatpp::network::ServerConnectionProvider> m_tcpConnectionProvider;
         std::shared_ptr<oatpp::network::Server> m_server;
         std::thread m_serverThread;

         std::map<std::string, std::string> m_aliases;

         std::shared_ptr<CHttpRequestHandlerFactory> m_httpRequestHandlerFactory;
         const boost::shared_ptr<std::vector<boost::shared_ptr<rest::service::IRestService>>> m_restServices;
      };
   } //namespace oatppServer
} //namespace web

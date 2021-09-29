#pragma once
#include <oatpp/network/ConnectionProvider.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>

#include "IAuthentication.h"
#include "HttpPages.h"
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
         //\param[in] useSsl              Determine if the server use a SSL certificate
         //\param[in] securedPort         Port to listen on for HTTPS browser requests e.g. 443
         //\param[in] docRoot             Path to folder containing html e.g. "./"
         //\param[in] restKeywordBase     The string which identifies a rest url ex: /rest/
         //\param[in] restServices        Vector of rest services
         //\param[in] allowExternalAccess Indicate if external access are allowed (in fact it add CORS headers to answer requests)
         //\param[in] aliases             Aliases from specific paths
         //                   Example configureAlias("/test/", "c:\\path\\to\\alias\\files\\")
         //                   -> then http://server:port/test/index.html will take c:\\path\\to\\alias\\files\\index.html
         //\param[in] authentication      Authentication handling
         CWebServer(const std::string& address,
                    unsigned short port,
                    bool useSsl,
                    unsigned short securedPort,
                    const boost::filesystem::path& docRoot,
                    const std::string& restKeywordBase,
                    boost::shared_ptr<std::vector<boost::shared_ptr<rest::service::IRestService>>> restServices,
                    const std::string& webSocketKeywordBase,
                    bool allowExternalAccess,
                    boost::shared_ptr<std::map<std::string, boost::filesystem::path>> aliases,
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
         std::shared_ptr<oatpp::network::Server> m_server;
         std::thread m_serverThread;

         boost::shared_ptr<std::map<std::string, boost::filesystem::path>> m_aliases;
         boost::shared_ptr<std::vector<boost::shared_ptr<rest::service::IRestService>>> m_restServices;
      };
   } //namespace oatppServer
} //namespace web

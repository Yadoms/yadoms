#pragma once
#include <oatpp/network/Server.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>

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
         //\param[in] useSSL              Determine if the server use a SSL certificate
         //\param[in] port                port to listen on for browser requests e.g. 8080 
         //\param[in] securedPort         port to listen on for HTTPS browser requests e.g. 443
         //\param[in] docRoot            path to folder containing html e.g. "./"
         //\param[in] restKeywordBase     the string which identifies a rest url ex: /rest/
         //\param[in] allowExternalAccess Indicate if external access are allowed (in fact it add CORS headers to answer requests)
         //
         CWebServer(const std::string& address,
                    bool useSSL,
                    unsigned short port,
                    unsigned short securedPort,
                    const std::string& docRoot,
                    const std::string& restKeywordBase,
                    const std::string& webSocketKeywordBase,
                    bool allowExternalAccess);
         ~CWebServer() override;

         // IWebServer implementation
         void start() override;
         void stop() override;
         IWebServerConfigurator* getConfigurator() override;
         // [END] IWebServer implementation

      private:
         std::shared_ptr<oatpp::web::server::HttpConnectionHandler> m_httpConnectionHandler;
         std::shared_ptr<oatpp::network::ServerConnectionProvider> m_tcpConnectionProvider;
         std::shared_ptr<oatpp::network::Server> m_server;
         std::thread m_serverThread;

         std::shared_ptr<CHttpRequestHandlerFactory> m_httpRequestHandlerFactory;
      };
   } //namespace oatppServer
} //namespace web
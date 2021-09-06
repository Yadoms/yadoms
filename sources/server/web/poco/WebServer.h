#pragma once

#include "web/IWebServer.h"
#include <Poco/Net/HTTPServer.h>
#include "HttpRequestHandlerFactory.h"

namespace web
{
   namespace poco
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
         //\param[in] doc_root            path to folder containing html e.g. "./"
         //\param[in] restKeywordBase     the string which identifies a rest url ex: /rest/
         //\param[in] allowExternalAccess Indicate if external access are allowed (in fact it add CORS headers to answer requests)
         //
         CWebServer(const std::string& address,
                    bool useSsl,
                    unsigned short port,
                    unsigned short securedPort,
                    const std::string& docRoot,
                    const std::string& restKeywordBase,
                    const std::string& webSocketKeywordBase,
                    bool allowExternalAccess);

         ~CWebServer() override;

         // IWebServer implementation
         IWebServerConfigurator* getConfigurator() override;
         void websiteHandlerAddAlias(const std::string& alias,
                                     const std::string& path) override;
         // [END] IWebServer implementation

         void start() const;
         void stop() const;

      private:
         boost::shared_ptr<Poco::Net::HTTPServer> m_embeddedWebServer;

         //Poco::SharedPtr is needed (not boost::shared_ptr)
         Poco::SharedPtr<CHttpRequestHandlerFactory> m_httpRequestHandlerFactory;
      };
   } //namespace poco
} //namespace web

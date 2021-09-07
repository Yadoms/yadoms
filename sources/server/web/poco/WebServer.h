#pragma once

#include "web/IWebServer.h"
#include <Poco/Net/HTTPServer.h>
#include "HttpRequestHandlerFactory.h"
#include "web/rest/service/IRestService.h"

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
         //\param[in] port                port to listen on for browser requests e.g. 8080 
         //\param[in] useSSL              Determine if the server use a SSL certificate
         //\param[in] securedPort         port to listen on for HTTPS browser requests e.g. 443
         //\param[in] doc_root            path to folder containing html e.g. "./"
         //\param[in] restKeywordBase     the string which identifies a rest url ex: /rest/
         //\param[in] allowExternalAccess Indicate if external access are allowed (in fact it add CORS headers to answer requests)
         //
         CWebServer(const std::string& address,
                    unsigned short port,
                    bool useSsl,
                    unsigned short securedPort,
                    const std::string& docRoot,
                    const std::string& restKeywordBase,
                    const std::string& webSocketKeywordBase,
                    bool allowExternalAccess);

         ~CWebServer() override;

         void restHandlerRegisterService(boost::shared_ptr<rest::service::IRestService> restService);
         void websiteHandlerAddAlias(const std::string& alias,
                                     const std::string& path);
         void configureAuthentication(boost::shared_ptr<authentication::IAuthentication> authenticator);

         void start() const;
         void stop() const;

      private:
         boost::shared_ptr<Poco::Net::HTTPServer> m_embeddedWebServer;

         //Poco::SharedPtr is needed (not boost::shared_ptr)
         Poco::SharedPtr<CHttpRequestHandlerFactory> m_httpRequestHandlerFactory;
      };
   } //namespace poco
} //namespace web

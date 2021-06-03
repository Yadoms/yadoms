#pragma once
#include <boost/smart_ptr/shared_ptr.hpp>
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

         //
         //\brief Descturtor
         //
         virtual ~CWebServer();

         // IWebServer implementation
         void start() override;
         void stop() override;
         IWebServerConfigurator* getConfigurator() override;
         // [END] IWebServer implementation

      private:
         //TODO boost::shared_ptr<Poco::Net::HTTPServer> m_embeddedWebServer;
      };
   } //namespace oatppServer
} //namespace web

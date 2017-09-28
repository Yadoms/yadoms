#pragma once

#include "web/IWebServer.h"
#include "web/rest/service/IRestService.h"
#include <Poco/Net/HTTPServer.h>
#include "HttpRequestHandlerFactory.h"

namespace web { namespace poco {

   //
   //\brief An embedded web server which supports REST api
   //
   class CWebServer : public IWebServer
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
      CWebServer(const std::string & address, const bool useSSL, const unsigned short port, const unsigned short securedPort, const std::string & doc_root, const std::string & restKeywordBase, const std::string & webSocketKeywordBase, bool allowExternalAccess);

      //
      //\brief Descturtor
      //
      virtual ~CWebServer();

      // IWebServer implementation
      virtual void start();
      virtual void stop();
      virtual IWebServerConfigurator* getConfigurator();
      // [END] IWebServer implementation

   private:
      boost::shared_ptr<Poco::Net::HTTPServer> m_embeddedWebServer;

      //Poco::SharedPtr is needed (not boost::shared_ptr)
      Poco::SharedPtr<CHttpRequestHandlerFactory> m_httpRequestHandlerFactory;
   };


} //namespace poco
} //namespace web


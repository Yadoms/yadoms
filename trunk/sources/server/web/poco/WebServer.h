#pragma once

#include "web/IWebServer.h"
#include "web/rest/service/IRestService.h"
#include "Poco/Net/HTTPServer.h"
#include "WebSocketRequestHandler.h"
#include "RestRequestHandler.h"
#include "WebsiteRequestHandler.h"

namespace web { namespace poco {

   //
   //\brief An embedded web server which supports REST api
   //
   class CWebServer : public IWebServer, public Poco::Net::HTTPRequestHandlerFactory
   {
   public:
      //
      //\brief              Create aa embedded w<eb server
      //\param[in] address  IP address.  In general, use  "0.0.0.0"
      //\param[in] port     port to listen on for browser requests e.g. "8080"
      //\param[in] doc_root path to folder containing html e.g. "./"
      //\param[in] restKeywordBase the string which identifies a rest url ex: /rest/
      //
      CWebServer(const std::string & address, const std::string & port, const std::string & doc_root, const std::string & restKeywordBase );

      //
      //\brief Descturtor
      //
      virtual ~CWebServer();

      // IWebServer implementation
      virtual void start();
      virtual void stop();
      virtual void registerRestService(boost::shared_ptr<web::rest::service::IRestService> restService);
      virtual void configureAlias(const std::string & alias, const std::string & path);
      // [END] IWebServer implementation

      // Poco::Net::HTTPRequestHandlerFactory implementation
      Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);
      // [END] Poco::Net::HTTPRequestHandlerFactory implementation

   private:
      std::string m_configAddress;
      std::string m_configPort;
      std::string m_configDocRoot;
      boost::shared_ptr<Poco::Net::HTTPServer> m_embeddedWebServer;

      boost::shared_ptr<CWebsiteRequestHandler> m_websiteRequestHandler;
      boost::shared_ptr<CRestRequestHandler> m_restRequestHandler;
      boost::shared_ptr<CWebSocketRequestHandler> m_webSocketRequestHandler;

      std::vector< boost::shared_ptr<web::rest::service::IRestService> > m_restService;

      //-------------------------------------
      ///\brief The aliases (alis, path to file)
      //-------------------------------------
      std::map<std::string, std::string> m_alias;
   };


} //namespace poco
} //namespace web


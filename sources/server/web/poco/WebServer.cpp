#include "stdafx.h"
#include "WebServer.h"
#include <shared/Log.h>

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"
#include "MimeType.h"

#include "WebSocketRequestHandler.h"
#include "RestRequestHandler.h"
#include "WebsiteRequestHandler.h"


namespace web { namespace poco {


   CWebServer::CWebServer(	   const std::string & address, const std::string & port, const std::string & doc_root, const std::string & restKeywordBase )
      :m_configAddress(address), m_configPort(port), m_configDocRoot(doc_root)
   {
      m_websiteRequestHandler.reset(new CWebsiteRequestHandler(doc_root));
      m_restRequestHandler.reset(new CRestRequestHandler(restKeywordBase));
      m_webSocketRequestHandler.reset(new CWebSocketRequestHandler);

      // set-up a server socket
      Poco::Net::ServerSocket svs(boost::lexical_cast<unsigned short>(port));
      // set-up a HTTPServer instance
      m_embeddedWebServer.reset(new Poco::Net::HTTPServer(this, svs, new Poco::Net::HTTPServerParams));


   }

   CWebServer::~CWebServer()
   {
      stop();
   }


   // IWebServer implementation
   void CWebServer::start()
   {
      // start the HTTPServer
      m_embeddedWebServer->start();
   }

   void CWebServer::stop()
   {
      if(m_embeddedWebServer.get())
         m_embeddedWebServer->stop();
   }

   void CWebServer::configureAlias(const std::string & alias, const std::string & path)
   {
      m_alias[alias] = path;
   }

   void CWebServer::registerRestService(boost::shared_ptr<web::rest::service::IRestService> restService)
   {
      m_restService.push_back(restService);
   }

   Poco::Net::HTTPRequestHandler* CWebServer::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
   {
      if (request.getURI() == "/ws")
         return new CWebSocketRequestHandler;
      else if (boost::istarts_with(request.getURI(), "/rest/"))
         //return m_restRequestHandler.get();
      {
         CRestRequestHandler * p = new CRestRequestHandler("/rest/");
         std::vector< boost::shared_ptr<web::rest::service::IRestService> >::iterator i;
         for (i = m_restService.begin(); i != m_restService.end(); ++i)
            p->registerRestService(*i);
         p->initialize();
         return p;
      }
      else
      {
         CWebsiteRequestHandler * p = new CWebsiteRequestHandler(m_configDocRoot);
         std::map<std::string, std::string>::iterator i;
         for (i = m_alias.begin(); i != m_alias.end();++i)
            p->configureAlias(i->first, i->second);
         return p;
      }
   }

} //namespace poco
} //namespace web


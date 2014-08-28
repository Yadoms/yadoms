#include "stdafx.h"
#include "HttpRequestHandlerFactory.h"
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


   CHttpRequestHandlerFactory::CHttpRequestHandlerFactory()
   {
   }

   CHttpRequestHandlerFactory::~CHttpRequestHandlerFactory()
   {
   }

   void CHttpRequestHandlerFactory::websiteHandlerConfigure(const std::string & doc_root)
   {
      m_configDocRoot = doc_root;
   }

   void CHttpRequestHandlerFactory::websiteHandlerAddAlias(const std::string & alias, const std::string & path)
   {
      m_alias[alias] = path;
   }

   void CHttpRequestHandlerFactory::restHandlerConfigure(const std::string & restKeywordBase)
   {
      m_restKeywordBase = restKeywordBase;
   }

   void CHttpRequestHandlerFactory::restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService> restService)
   {
      m_restService.push_back(restService);
   }

   void CHttpRequestHandlerFactory::webSocketConfigure(const std::string & webSocketKeyword)
   {
      m_webSocketKeyword = webSocketKeyword;
   }

   Poco::Net::HTTPRequestHandler* CHttpRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
   {
      if (request.getURI() == m_webSocketKeyword)
         return new CWebSocketRequestHandler;
      else if (boost::istarts_with(request.getURI(), m_restKeywordBase))
         //return m_restRequestHandler.get();
      {
         CRestRequestHandler * p = new CRestRequestHandler(m_restKeywordBase);
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


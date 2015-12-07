#include "stdafx.h"
#include "HttpRequestHandlerFactory.h"
#include <shared/Log.h>

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/WebSocket.h>
#include <Poco/Net/NetException.h>
#include "MimeType.h"


#include "WebSocketRequestHandler.h"
#include "RestRequestHandler.h"
#include "WebsiteRequestHandler.h"
#include "AuthenticationRequestHandler.h"
#include "HttpRequestHandlerContainer.h"

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

   void CHttpRequestHandlerFactory::restHandlerRegisterService(boost::shared_ptr<rest::service::IRestService> restService)
   {
      m_restService.push_back(restService);
   }

   void CHttpRequestHandlerFactory::webSocketConfigure(const std::string & webSocketKeyword)
   {
      m_webSocketKeyword = webSocketKeyword;
   }

   void CHttpRequestHandlerFactory::configureAuthentication(boost::shared_ptr<authentication::IAuthentication> authenticator)
   {
      m_authenticator = authenticator;
   }
   
   
   Poco::Net::HTTPRequestHandler* CHttpRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
   {
      if (boost::istarts_with(request.getURI(), m_webSocketKeyword))
      {
         //manage WebSocket requests
         return createWebSocketRequestHandler();
      }

      if (boost::istarts_with(request.getURI(), m_restKeywordBase))
      {
         //manage REST requests
         return createRestRequestHandler();
      }

      //manage HTTP requests
      return createHttpRequestHandler();
   }


   Poco::Net::HTTPRequestHandler* CHttpRequestHandlerFactory::createWebSocketRequestHandler()
   {
      //WebSockets are persistents connections, so recreate request handler for each request
      CWebSocketRequestHandler * realRequestHandler = new CWebSocketRequestHandler();
      if (m_authenticator)
         return new CAuthenticationRequestHandler(m_authenticator, boost::shared_ptr<Poco::Net::HTTPRequestHandler>(realRequestHandler), false);
      return realRequestHandler;
   }

   Poco::Net::HTTPRequestHandler* CHttpRequestHandlerFactory::createRestRequestHandler()
   {
      //Rest request are non persistents, so could use the same object for all requests
      boost::shared_ptr<Poco::Net::HTTPRequestHandler> realRequestHandler;
      if (!m_restRequestHandler)
      {
         m_restRequestHandler = boost::make_shared<CRestRequestHandler>(m_restKeywordBase, m_restService);
      }
      realRequestHandler = m_restRequestHandler;

      if (m_authenticator)
      {
         if (!m_restRequestHandlerWithAuthentication)
            m_restRequestHandlerWithAuthentication = boost::make_shared<CAuthenticationRequestHandler>(m_authenticator, m_restRequestHandler, false);
         realRequestHandler = m_restRequestHandlerWithAuthentication;
      }
      return new CHttpRequestHandlerContainer(realRequestHandler);
   }

   Poco::Net::HTTPRequestHandler* CHttpRequestHandlerFactory::createHttpRequestHandler()
   {
      //Http request are non persistents, so could use the same object for all requests
      boost::shared_ptr<Poco::Net::HTTPRequestHandler> realRequestHandler;
      if (!m_httpRequestHandler)
      {
         m_httpRequestHandler = boost::make_shared<CWebsiteRequestHandler>(m_configDocRoot, m_alias);
      }
      realRequestHandler = m_httpRequestHandler;

      if (m_authenticator)
      {
         if (!m_httpRequestHandlerWithAuthentication)
            m_httpRequestHandlerWithAuthentication = boost::make_shared<CAuthenticationRequestHandler>(m_authenticator, m_httpRequestHandler, true);
         realRequestHandler = m_httpRequestHandlerWithAuthentication;
      }

      return new CHttpRequestHandlerContainer(realRequestHandler);
   }

} //namespace poco
} //namespace web


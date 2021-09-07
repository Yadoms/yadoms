#include "stdafx.h"
#include "HttpRequestHandlerFactory.h"
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include "AuthenticationRequestHandler.h"
#include "HttpRequestHandlerContainer.h"
#include "RestRequestHandler.h"
#include "WebsiteRequestHandler.h"
#include "WebSocketRequestHandler.h"

namespace web
{
   namespace poco
   {
      CHttpRequestHandlerFactory::CHttpRequestHandlerFactory()
         : m_allowExternalAccess(false)
      {
      }

      CHttpRequestHandlerFactory::~CHttpRequestHandlerFactory()
      {
      }

      void CHttpRequestHandlerFactory::websiteHandlerConfigure(const std::string& docRoot)
      {
         m_configDocRoot = docRoot;
      }

      void CHttpRequestHandlerFactory::websiteHandlerAddAlias(const std::string& alias, const std::string& path)
      {
         m_alias[alias] = path;
      }

      void CHttpRequestHandlerFactory::restHandlerConfigure(const std::string& restKeywordBase)
      {
         m_restKeywordBase = restKeywordBase;
      }

      void CHttpRequestHandlerFactory::restHandlerRegisterService(boost::shared_ptr<rest::service::IRestService> restService)
      {
         m_restService.push_back(restService);
      }

      void CHttpRequestHandlerFactory::webSocketConfigure(const std::string& webSocketKeyword)
      {
         m_webSocketKeyword = webSocketKeyword;
      }

      void CHttpRequestHandlerFactory::allowExternalAccess(bool allowExternalAccess)
      {
         m_allowExternalAccess = allowExternalAccess;
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
         const auto realRequestHandler = new CWebSocketRequestHandler();
         return realRequestHandler;
      }

      Poco::Net::HTTPRequestHandler* CHttpRequestHandlerFactory::createRestRequestHandler()
      {
         if (!m_restRequestHandler)
         {
            m_restRequestHandler = boost::make_shared<CRestRequestHandler>(m_restKeywordBase, m_restService);
         }
         auto realRequestHandler = m_restRequestHandler;

         if (m_authenticator)
         {
            if (!m_restRequestHandlerWithAuthentication)
               m_restRequestHandlerWithAuthentication = boost::make_shared<
                  CAuthenticationRequestHandler>(m_authenticator, m_restRequestHandler, true);
            realRequestHandler = m_restRequestHandlerWithAuthentication;
         }
         return new CHttpRequestHandlerContainer(realRequestHandler, m_allowExternalAccess);
      }

      Poco::Net::HTTPRequestHandler* CHttpRequestHandlerFactory::createHttpRequestHandler()
      {
         if (!m_httpRequestHandler)
         {
            m_httpRequestHandler = boost::make_shared<CWebsiteRequestHandler>(m_configDocRoot, m_alias);
         }
         auto realRequestHandler = m_httpRequestHandler;

         if (m_authenticator)
         {
            if (!m_httpRequestHandlerWithAuthentication)
               m_httpRequestHandlerWithAuthentication = boost::make_shared<
                  CAuthenticationRequestHandler>(m_authenticator, m_httpRequestHandler, true);
            realRequestHandler = m_httpRequestHandlerWithAuthentication;
         }

         return new CHttpRequestHandlerContainer(realRequestHandler, m_allowExternalAccess);
      }
   } //namespace poco
} //namespace web

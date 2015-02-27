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

namespace web { namespace poco {


   CHttpRequestHandlerFactory::CHttpRequestHandlerFactory(boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter)
      :m_notificationCenter(notificationCenter)
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
   
   Poco::Net::HTTPRequestHandler* CHttpRequestHandlerFactory::createInternalRequestHandler(const Poco::Net::HTTPServerRequest& request)
   {
      if (boost::istarts_with(request.getURI(), m_webSocketKeyword))
         return new CWebSocketRequestHandler(m_notificationCenter);

      if (boost::istarts_with(request.getURI(), m_restKeywordBase))
      {
         CRestRequestHandler * p = new CRestRequestHandler(m_restKeywordBase);
         std::vector< boost::shared_ptr<rest::service::IRestService> >::iterator i;
         for (i = m_restService.begin(); i != m_restService.end(); ++i)
            p->registerRestService(*i);
         p->initialize();
         return p;
      }
      
      CWebsiteRequestHandler * p = new CWebsiteRequestHandler(m_configDocRoot);
      std::map<std::string, std::string>::iterator i;
      for (i = m_alias.begin(); i != m_alias.end();++i)
         p->configureAlias(i->first, i->second);
      return p;      
   }
   
   Poco::Net::HTTPRequestHandler* CHttpRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
   {
      Poco::Net::HTTPRequestHandler* realRequesHandler = createInternalRequestHandler(request);
      if(m_authenticator)
      {
         return new CAuthenticationRequestHandler(m_authenticator, boost::shared_ptr<Poco::Net::HTTPRequestHandler>(realRequesHandler));
      }
      else
      {
         return realRequesHandler;
      }
   }
} //namespace poco
} //namespace web


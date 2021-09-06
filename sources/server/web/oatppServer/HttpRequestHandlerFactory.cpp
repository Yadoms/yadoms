#include "stdafx.h"
#include "HttpRequestHandlerFactory.h"

namespace web
{
   namespace oatppServer
   {
      CHttpRequestHandlerFactory::CHttpRequestHandlerFactory()
         : m_allowExternalAccess(false)
      {
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

      void CHttpRequestHandlerFactory::configureAuthentication(boost::shared_ptr<authentication::IAuthentication> authenticator)
      {
         m_authenticator = authenticator;
      }

      void CHttpRequestHandlerFactory::allowExternalAccess(bool allowExternalAccess)
      {
         m_allowExternalAccess = allowExternalAccess;
      }
   } //namespace oatppServer
} //namespace web

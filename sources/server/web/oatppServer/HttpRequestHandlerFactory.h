#pragma once

#include "web/IWebServerConfigurator.h"

namespace web
{
   namespace oatppServer
   {
      //==============================================
      ///\brief The factory which handles http requests
      //==============================================
      class CHttpRequestHandlerFactory final : public IWebServerConfigurator //TODO conserver ? (cette classe n'a aucune plus-value)
      {
      public:
         CHttpRequestHandlerFactory();
         ~CHttpRequestHandlerFactory() override = default;

         // IWebServerConfigurator implementation
         void websiteHandlerConfigure(const std::string& docRoot) override;
         void websiteHandlerAddAlias(const std::string& alias, const std::string& path) override;
         void restHandlerConfigure(const std::string& restKeywordBase) override;
         void restHandlerRegisterService(boost::shared_ptr<rest::service::IRestService> restService) override;
         void webSocketConfigure(const std::string& webSocketKeyword) override;
         void configureAuthentication(boost::shared_ptr<authentication::IAuthentication> authenticator) override;
         void allowExternalAccess(bool allowExternalAccess) override;
         // [END] IWebServerConfigurator implementation

      private:
         std::string m_configDocRoot;
         std::string m_restKeywordBase;
         std::string m_webSocketKeyword;
         std::vector<boost::shared_ptr<rest::service::IRestService>> m_restService;
         std::map<std::string, std::string> m_alias;
         boost::shared_ptr<authentication::IAuthentication> m_authenticator;
         bool m_allowExternalAccess;
      };
   } //namespace oatppServer
} //namespace web
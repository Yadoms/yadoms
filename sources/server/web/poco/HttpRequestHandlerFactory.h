#pragma once
#include <Poco/Net/HTTPServer.h>

#include "web/rest/service/IRestService.h"
#include "authentication/IAuthentication.h"

namespace web
{
   namespace poco
   {
      //==============================================
      ///\brief The factiry which  handles http requests
      //==============================================
      class CHttpRequestHandlerFactory final : public Poco::Net::HTTPRequestHandlerFactory
      {
      public:
         CHttpRequestHandlerFactory();
         ~CHttpRequestHandlerFactory() override;

         // Poco::Net::HTTPRequestHandlerFactory implementation
         Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;
         // [END] Poco::Net::HTTPRequestHandlerFactory implementation

         void websiteHandlerConfigure(const std::string& docRoot);
         void websiteHandlerAddAlias(const std::string& alias,
                                     const std::string& path);
         void restHandlerConfigure(const std::string& restKeywordBase);
         void webSocketConfigure(const std::string& webSocketKeyword);
         void restHandlerRegisterService(boost::shared_ptr<rest::service::IRestService> restService);
         void allowExternalAccess(bool allowExternalAccess);
         void configureAuthentication(boost::shared_ptr<authentication::IAuthentication> authenticator);

      private:
         //==============================================
         /// \brief Create the websocket request handler
         /// \return       the request handler
         //==============================================
         static Poco::Net::HTTPRequestHandler* createWebSocketRequestHandler();

         //==============================================
         /// \brief Create the REST request handler
         /// \return       the request handler
         //==============================================
         Poco::Net::HTTPRequestHandler* createRestRequestHandler();

         //==============================================
         /// \brief Create the HTTP request handler
         /// \return       the request handler
         //==============================================
         Poco::Net::HTTPRequestHandler* createHttpRequestHandler();

         std::string m_configDocRoot;
         std::string m_restKeywordBase;
         std::string m_webSocketKeyword;
         std::vector<boost::shared_ptr<rest::service::IRestService>> m_restService;
         std::map<std::string, std::string> m_alias;
         boost::shared_ptr<authentication::IAuthentication> m_authenticator;
         bool m_allowExternalAccess;

         boost::shared_ptr<Poco::Net::HTTPRequestHandler> m_restRequestHandler;
         boost::shared_ptr<Poco::Net::HTTPRequestHandler> m_restRequestHandlerWithAuthentication;
         boost::shared_ptr<Poco::Net::HTTPRequestHandler> m_httpRequestHandler;
         boost::shared_ptr<Poco::Net::HTTPRequestHandler> m_httpRequestHandlerWithAuthentication;
      };
   } //namespace poco
} //namespace web

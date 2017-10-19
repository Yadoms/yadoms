#pragma once

#include <Poco/Net/HTTPServer.h>
#include "web/IWebServerConfigurator.h"

namespace web
{
   namespace poco
   {
      //==============================================
      ///\brief The factiry which  handles http requests
      //==============================================
      class CHttpRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory, public IWebServerConfigurator
      {
      public:
         //==============================================
         ///\brief Constructor
         //==============================================
         CHttpRequestHandlerFactory();

         //==============================================
         ///\brief Destructor
         //==============================================
         virtual ~CHttpRequestHandlerFactory();

         // IWebServerConfigurator implementation
         void websiteHandlerConfigure(const std::string& doc_root) override;
         void websiteHandlerAddAlias(const std::string& alias, const std::string& path) override;
         void restHandlerConfigure(const std::string& restKeywordBase) override;
         void restHandlerRegisterService(boost::shared_ptr<rest::service::IRestService> restService) override;
         void webSocketConfigure(const std::string& webSocketKeyword) override;
         void configureAuthentication(boost::shared_ptr<authentication::IAuthentication> authenticator) override;
         void allowExternalAccess(bool allowExternalAccess) override;
         // [END] IWebServerConfigurator implementation

         // Poco::Net::HTTPRequestHandlerFactory implementation
         Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);
         // [END] Poco::Net::HTTPRequestHandlerFactory implementation

      private:
         //==============================================
         /// \brief Create the websocket request handler
         /// \return       the request handler
         //==============================================
         Poco::Net::HTTPRequestHandler* createWebSocketRequestHandler() const;

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

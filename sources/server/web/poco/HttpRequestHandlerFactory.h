#pragma once

#include <Poco/Net/HTTPServer.h>
#include "web/IWebServerConfigurator.h"
#include <shared/notification/NotificationCenter.h>

namespace web { namespace poco {

   //==============================================
   ///\brief The factiry which  handles http requests
   //==============================================
   class CHttpRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory, public IWebServerConfigurator
   {
   public:
      //==============================================
      ///\brief Constructor
      /// \param[in]    notificationCenter The notification center
      //==============================================
      CHttpRequestHandlerFactory(boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter);

      //==============================================
      ///\brief Destructor
      //==============================================
      virtual ~CHttpRequestHandlerFactory();

      // IWebServerConfigurator implementation
      virtual void websiteHandlerConfigure(const std::string & doc_root);
      virtual void websiteHandlerAddAlias(const std::string & alias, const std::string & path);
      virtual void restHandlerConfigure(const std::string & restKeywordBase);
      virtual void restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService> restService);
      virtual void webSocketConfigure(const std::string & webSocketKeyword);
      virtual void configureAuthentication(boost::shared_ptr<authentication::IAuthentication> authenticator);
      // [END] IWebServerConfigurator implementation

      // Poco::Net::HTTPRequestHandlerFactory implementation
      Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);
      // [END] Poco::Net::HTTPRequestHandlerFactory implementation

   private:
      std::string m_configDocRoot;
      std::string m_restKeywordBase;
      std::string m_webSocketKeyword;
      std::vector< boost::shared_ptr<web::rest::service::IRestService> > m_restService;
      std::map<std::string, std::string> m_alias;
      boost::shared_ptr<shared::notification::CNotificationCenter> m_notificationCenter;
      boost::shared_ptr<authentication::IAuthentication> m_authenticator;
   };


} //namespace poco
} //namespace web


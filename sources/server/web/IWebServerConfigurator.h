#pragma once

#include "IRestHandler.h"
#include "authentication/IAuthentication.h"

namespace web
{
   //
   //\brief Interface for web server configurator
   //
   class IWebServerConfigurator //TODO virer (ne sert que pour Poco, donc pas d'interface nécessaire)
   {
   public:
      virtual ~IWebServerConfigurator() = default;

      //-----------------------------------------
      ///\brief      Configure the REST service handler
      ///\param [in] restKeywordBase the REST keyword which permit to determine rest queries (i.e.: /rest/)
      //-----------------------------------------
      virtual void restHandlerConfigure(const std::string& restKeywordBase) = 0;

      //-----------------------------------------
      ///\brief      Register a REST service
      ///\param [in] restService the REST service to register
      //-----------------------------------------
      virtual void restHandlerRegisterService(boost::shared_ptr<rest::service::IRestService> restService) = 0;

      //-----------------------------------------
      ///\brief      Configure the WebSocket service handler
      ///\param [in] webSocketKeyword  the keyword which permit to determine websocket queries (i.e.: ws/)
      //-----------------------------------------
      virtual void webSocketConfigure(const std::string& webSocketKeyword) = 0;

      //-----------------------------------------
      ///\brief      Configure the webserver to use authentication
      ///\param [in] authenticator  the authentication provider
      //-----------------------------------------
      virtual void configureAuthentication(boost::shared_ptr<authentication::IAuthentication> authenticator) = 0;

      //-----------------------------------------
      ///\brief      Allow external access
      ///\param [in] allowExternalAccess  true if externals access are allowed (android app for example)
      //-----------------------------------------
      virtual void allowExternalAccess(bool allowExternalAccess) =0;
   };
} //namespace web

#pragma once

#include "IRestHandler.h"
#include "authentication/IAuthentication.h"

namespace web {
   //
   //\brief Interface for web server configurator
   //
   class IWebServerConfigurator
   {
   public:
      //-----------------------------------------
      ///\brief      Virtual destructor
      //-----------------------------------------
      virtual ~IWebServerConfigurator()
      {
      }

      //-----------------------------------------
      ///\brief      Configure the website handler
      ///\param [in] doc_root    the path to files
      //-----------------------------------------
      virtual void websiteHandlerConfigure(const std::string & doc_root) = 0;

      //--------------------------------------
      ///\brief   Configure an alias for the website handler
      ///\param [in]    alias : the alias used (must start and end with /)
      ///\param [in]    path : the path of the alias file
      ///
      /// Example configureAlias("/test/", "c:\\path\\to\\alias\\files\\")
      ///         -> then http://server:port/test/index.html will take c:\\path\\to\\alias\\files\\index.html
      //--------------------------------------
      virtual void websiteHandlerAddAlias(const std::string & alias, const std::string & path) = 0;

      //-----------------------------------------
      ///\brief      Configure the REST service handler
      ///\param [in] restKeywordBase the REST keyword which permit to determine rest queries (i.e.: /rest/)
      //-----------------------------------------
      virtual void restHandlerConfigure(const std::string & restKeywordBase) = 0;

      //-----------------------------------------
      ///\brief      Register a REST service
      ///\param [in] restService the REST service to register
      //-----------------------------------------
      virtual void restHandlerRegisterService(boost::shared_ptr<web::rest::service::IRestService> restService) = 0;

      //-----------------------------------------
      ///\brief      Configure the WebSocket service handler
      ///\param [in] webSocketKeyword  the keyword which permit to determine websocket queries (i.e.: ws/)
      //-----------------------------------------
      virtual void webSocketConfigure(const std::string & webSocketKeyword) = 0;
      
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
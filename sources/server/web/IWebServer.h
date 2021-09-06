#pragma once
#include "IWebServerConfigurator.h"

namespace web
{
   //
   //\Brief Interface for web server
   //
   class IWebServer
   {
   public:
      virtual ~IWebServer() = default;

      //-----------------------------------------
      ///\brief      Get the web server configurator
      ///\return     An object which permit the webserver configuration
      //-----------------------------------------
      virtual IWebServerConfigurator* getConfigurator() = 0; //TODO virer

      //--------------------------------------
      ///\brief   Configure an alias for the website handler
      ///\param [in]    alias : the alias used (must start and end with /)
      ///\param [in]    path : the path of the alias file
      ///
      /// Example configureAlias("/test/", "c:\\path\\to\\alias\\files\\")
      ///         -> then http://server:port/test/index.html will take c:\\path\\to\\alias\\files\\index.html
      //--------------------------------------
      virtual void websiteHandlerAddAlias(const std::string& alias,
                                          const std::string& path) =0;
   };
} //namespace web

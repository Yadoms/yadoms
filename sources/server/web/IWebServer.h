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

      //
      //\Brief Start the webserver
      //
      virtual void start() = 0;

      //
      //\Brief Stop the webserver
      //
      virtual void stop() = 0;


      //-----------------------------------------
      ///\brief      Get the web server configurator
      ///\return     An object which permit the webserver configuration
      //-----------------------------------------
      virtual IWebServerConfigurator* getConfigurator() = 0;
   };
} //namespace web

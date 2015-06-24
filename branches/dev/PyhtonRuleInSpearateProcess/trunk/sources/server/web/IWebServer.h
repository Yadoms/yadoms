#pragma once

#include "IRestHandler.h"
#include <Poco/SharedPtr.h>
#include "IWebServerConfigurator.h"

namespace web {
   //
   //\brief Interface for web server
   //
   class IWebServer
   {
   public:
      virtual ~IWebServer()
      {
      }

      //
      //\brief Start the webserver
      //
      virtual void start() = 0;

      //
      //\brief Stop the webserver
      //
      virtual void stop() = 0;


      //-----------------------------------------
      ///\brief      Get the web server configurator
      ///\return     An object which permit the webserver configuration
      //-----------------------------------------
      virtual IWebServerConfigurator* getConfigurator() = 0;
   };

} //namespace web
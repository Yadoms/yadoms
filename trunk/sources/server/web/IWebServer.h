#pragma once

#include "IRestHandler.h"

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


      //--------------------------------------
      ///\brief   Get the REST handler
      ///\return  A shared ptr to a rest handler. It could be boost::shared_ptr<IRestHandler>(NULL) if the instance do not support REST
      //--------------------------------------
      virtual boost::shared_ptr<IRestHandler> getRestHandler() = 0;

      //--------------------------------------
      ///\brief   Configure an alias
      ///\param [in]    alias : the alias used (must start and end with /)
      ///\param [in]    path : the path of the alias file
      ///
      /// Example configureAlias("/test/", "c:\\path\\to\\alias\\files\\")
      ///         -> then http://server:port/test/index.html will take c:\\path\\to\\alias\\files\\index.html
      //--------------------------------------
      virtual void configureAlias(const std::string & alias, const std::string & path) = 0;
   };

} //namespace web
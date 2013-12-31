#pragma once

#include "IRestHandler.h"

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
   ///\return  A shared ptr to a rest handler. It could be boost::shared_ptr<IRestHandler>(NULL) if the insace do not support REST
   //--------------------------------------
   virtual boost::shared_ptr<IRestHandler> getRestHandler() = 0;
};


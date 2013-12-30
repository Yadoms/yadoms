#pragma once

#include "rest/IRestService.h"

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
   ///\brief   Configure a REST service handler
   ///\param [in] restKeyName   the rest key name
   ///\param [in] restService   the rest handler
   //-----------------------------------------
   virtual void configureRestService(boost::shared_ptr<IRestService> restService) = 0;
};



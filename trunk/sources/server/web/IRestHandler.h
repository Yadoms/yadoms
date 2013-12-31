#pragma once

#include "rest/IRestService.h"

//
//\brief Interface for web server
//
class IRestHandler
{
public:
	virtual ~IRestHandler()
	{
	}

   //-----------------------------------------
   ///\brief   Configure a REST service handler
   ///\param [in] restKeyName   the rest key name
   ///\param [in] restService   the rest handler
   //-----------------------------------------
   virtual void configureRestService(boost::shared_ptr<IRestService> restService) = 0;
};
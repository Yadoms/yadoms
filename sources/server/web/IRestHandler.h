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
   ///\brief   Initialize the REST services
   //-----------------------------------------
   virtual void initialize() = 0;

   //-----------------------------------------
   ///\brief      Register a REST service
   ///\param [in] restService the REST service to register
   //-----------------------------------------
   virtual void registerRestService(boost::shared_ptr<IRestService> restService) =0;
};
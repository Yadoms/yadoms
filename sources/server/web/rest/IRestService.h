#pragma once


#include "json/Json.h"
#include "RestDispatcher.h"

//----------------------------
///\brief   Interface for REST service
//----------------------------
class IRestService
{
public:
   //--------------------------------------   
   ///\brief   Method called to initialize the dispatcher
   //-------------------------------------- 
   virtual void configureDispatcher(CRestDispatcher & dispatcher) = 0;

   //----------------------------
   ///\brief   Destructor
   //----------------------------
   virtual ~IRestService() {}
};


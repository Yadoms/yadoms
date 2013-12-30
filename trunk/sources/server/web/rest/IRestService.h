#pragma once


#include "json/json.h"

//----------------------------
///\brief   Interface for REST service
//----------------------------
class IRestService
{
public:
   //----------------------------
   ///\brief   Get the REST keyword
   ///\return  the REST keyword
   //----------------------------
   virtual const std::string & getRestKeyword() = 0;

   //----------------------------
   ///\brief   Destructor
   //----------------------------
   virtual ~IRestService() {}
};


//----------------------------
///\brief   Interface for REST read service
//----------------------------
class IReadRestService : public IRestService
{
public:
   //----------------------------
   ///\brief   Read an object
   ///\param [in] objectId   the object id to read
   ///\return  the  JSON result
   //----------------------------
   virtual CJson readObject(const std::string & objectId) = 0;   
      
   //----------------------------
   ///\brief   Read all objects
   ///\param [in] parameter   the  JSON parameter which identifies the objects to read
   ///\return  the  JSON result
   //----------------------------
   virtual CJson readObjects() = 0;   


public:
   //----------------------------
   ///\brief   Destructor
   //----------------------------
   virtual ~IReadRestService() {}
};

//----------------------------
///\brief   Interface for REST write service
//----------------------------
class IWriteRestService : public IRestService
{
public:
   //----------------------------
   ///\brief   Create an object
   ///\param [in] parameter   the  JSON parameter which describes the object to create
   ///\return  the  JSON result
   //----------------------------
   virtual CJson createObject(const CJson & parameter) = 0;   
   
   //----------------------------
   ///\brief   Update an object
   ///\param [in] parameter   the  JSON parameter which describes the object to update with its modification
   ///\return  the  JSON result
   //----------------------------
  virtual CJson updateObject(const CJson & parameter) = 0;   
   
   //----------------------------
   ///\brief   Delete an object
   ///\param [in] parameter   the  JSON parameter which describes the object to delete
   ///\return  the  JSON result
   //----------------------------
   virtual CJson deleteObject(const CJson & parameter) = 0;   

public:
   //----------------------------
   ///\brief   Destructor
   //----------------------------
   virtual ~IWriteRestService() {}
};

//----------------------------
///\brief   Interface for RESTFull service
//----------------------------
class IFullRestService : public IReadRestService, public IWriteRestService
{
public:
   //----------------------------
   ///\brief   Destructor
   //----------------------------
   virtual ~IFullRestService() {}
};
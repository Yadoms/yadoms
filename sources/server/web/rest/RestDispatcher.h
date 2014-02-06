#pragma once

#include "json/json.h"

class CRestDispatcher
{
public:
   //--------------------------------------   
   ///\brief   Constructor
   //-------------------------------------- 
   CRestDispatcher();

   //--------------------------------------   
   ///\brief   Destructor
   //-------------------------------------- 
   virtual ~CRestDispatcher();

   //--------------------------------------   
   ///\brief         dispatch url 
   ///\param [in]    requestType       the type of request, usually GET, PUT, POST or DELETE
   ///\param [in]    url               the utrl keywords (each entry is url split with /)
   ///\param [in]    requestContent    request content (put, post or delete request)
   ///\return        the data in Json format
   //-------------------------------------- 
   CJson dispath(const std::string & requestType, const std::vector<std::string> & url, const CJson & requestContent);

   //--------------------------------------   
   ///\brief   Define an Url pattern /widget/*/acquisitions (=> widget, *, acquisitions)
   //--------------------------------------     
   typedef std::vector<std::string> CUrlPattern;

   //--------------------------------------   
   ///\brief   Define a function pointer on a method like CJson readData(const std::vector<std::string> & )
   //--------------------------------------  
   typedef boost::function2<CJson, const std::vector<std::string> &, const CJson & > CRestMethodHandler;

   //--------------------------------------   
   ///\brief         register a rest handler
   ///\param [in]    requestType       the type of request, usually GET, PUT, POST or DELETE
   ///\param [in]    urlPattern        the url pattern : ie. : /widget/*/acquisitions
   ///\param [in]    functionPtr       the function pointer to call when url match configuration pattern
   //-------------------------------------- 
   void registerRestMethodHandler(const std::string & requestType, const CUrlPattern & urlPattern, CRestMethodHandler functionPtr);

private:
   //--------------------------------------   
   ///\brief         check if an url match a pattern
   ///\param [in]    url               the url to check
   ///\param [in]    urlPattern        the pattern
   ///\return        true if the url match the pattern
   //-------------------------------------- 
   const bool match(const std::vector<std::string> & url, const CUrlPattern & urlPattern);

   //--------------------------------------   
   ///\brief         Contains all the rest method pointers with their url pattern
   //--------------------------------------   
   std::map<std::string, std::map<CUrlPattern, CRestMethodHandler> > m_handledFunctions;
};


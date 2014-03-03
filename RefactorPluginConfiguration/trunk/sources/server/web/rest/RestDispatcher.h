#pragma once

#include "json/Json.h"

namespace web { namespace rest {


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
      web::rest::json::CJson dispath(const std::string & requestType, const std::vector<std::string> & url, const web::rest::json::CJson & requestContent);

      //--------------------------------------   
      ///\brief   Define an Url pattern /widget/*/acquisitions (=> widget, *, acquisitions)
      //--------------------------------------     
      typedef std::vector<std::string> CUrlPattern;

      //--------------------------------------   
      ///\brief   Define a function pointer on a method like web::rest::json::CJson readData(const std::vector<std::string> & )
      //--------------------------------------  
      typedef boost::function2<web::rest::json::CJson, const std::vector<std::string> &, const web::rest::json::CJson & > CRestMethodHandler;

      //--------------------------------------   
      ///\brief   Define a function pointer on a method like web::rest::json::CJson readData(const std::vector<std::string> & )
      //--------------------------------------  
      typedef boost::function3<web::rest::json::CJson, CRestMethodHandler, const std::vector<std::string> &, const web::rest::json::CJson & > CRestMethodIndirector;

      //--------------------------------------   
      ///\brief         register a rest handler
      ///\param [in]    requestType       the type of request, usually GET, PUT, POST or DELETE
      ///\param [in]    urlPattern        the url pattern : ie. : /widget/*/acquisitions
      ///\param [in]    functionPtr       the function pointer to call when url match configuration pattern
      ///\param [in]    indirectPtr       the function pointer which will call the functionPtr (indirect so some common process can be handled in this function)
      //-------------------------------------- 
      void registerRestMethodHandler(const std::string & requestType, const CUrlPattern & urlPattern, CRestMethodHandler functionPtr, CRestMethodIndirector indirectPtr = NULL);

   private:
      //--------------------------------------   
      ///\brief         check if an url match a pattern
      ///\param [in]    url               the url to check
      ///\param [in]    urlPattern        the pattern
      ///\param [in]    allowWildcard     if true wildcard are allowed (if not, pattern with wilcard are excluded)
      ///\return        true if the url match the pattern
      //-------------------------------------- 
      const bool match(const std::vector<std::string> & url, const CUrlPattern & urlPattern, const bool allowWildcard);

      //--------------------------------------   
      ///\brief         call the registered method
      ///\param [in]    realMethod     the real rest method
      ///\param [in]    encapsulatedMethod a method which aims is to call realMethod (can be null for direct call to realMethod)
      ///\param [in]    url               the url to check
      ///\param [in]    urlPattern        the pattern
      ///\return        the data in Json format
      //-------------------------------------- 
      web::rest::json::CJson callRealMethod(CRestMethodHandler realMethod, CRestMethodIndirector encapsulatedMethod, const std::vector<std::string> & url, const web::rest::json::CJson & urlPattern);

      //--------------------------------------
      ///\brief         Contains all the rest method pointers with their url pattern
      //--------------------------------------
      typedef std::map<CUrlPattern, std::pair<CRestMethodHandler, CRestMethodIndirector> > RestMethodMap;
      std::map<std::string, RestMethodMap> m_handledFunctions;
   };

} //namespace rest
} //namespace web 


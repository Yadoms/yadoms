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
      ///\param [in]    url               the url keywords (each entry is url split with /)
      ///\param [in]    requestContent    request content (put, post or delete request)
      ///\return        the data in Json format
      //-------------------------------------- 
      web::rest::json::CJson dispath(const std::string & requestType, const std::vector<std::string> & url, const web::rest::json::CJson & requestContent);

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
      void registerRestMethodHandler(const std::string & requestType,  const std::vector<std::string> & urlPattern, CRestMethodHandler functionPtr, CRestMethodIndirector indirectPtr = NULL);

      //--------------------------------------   
      ///\brief Sort the methods to allow a good search
      //--------------------------------------   
      void sortRestMethod();

   private:
      //--------------------------------------   
      ///\brief   A container class for an url pattern with iths function pointers
      /// /widget/*/acquisitions (=> widget, *, acquisitions)
      //-------------------------------------
      class CUrlPattern
      {
      public:
         CUrlPattern(const std::vector<std::string> & pattern, CRestMethodHandler & handler, CRestMethodIndirector & indirector);
         virtual ~CUrlPattern();
         const std::vector<std::string> & getPattern() const;
         const CRestMethodHandler & getMethodHandler() const;
         const CRestMethodIndirector & getMethodIndirector() const;
         bool operator<(const CUrlPattern &right) const;
         std::string toString() const;
      private:
         std::vector<std::string> m_pattern;
         CRestMethodHandler m_methodHandler;
         CRestMethodIndirector m_methodIndirector;
      };

      //--------------------------------------   
      ///\brief         check if an url match a pattern
      ///\param [in]    url               the url to check
      ///\param [in]    urlPattern        the pattern
      ///\return        true if the url match the pattern
      //-------------------------------------- 
      const bool match(const std::vector<std::string> & url, const CUrlPattern & urlPattern);

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
      ///\brief   All the registered handle
      //--------------------------------------   
      typedef std::vector<CUrlPattern> RestMethodMap;
      std::map<std::string, RestMethodMap > m_handledFunctions;
   };

} //namespace rest
} //namespace web 


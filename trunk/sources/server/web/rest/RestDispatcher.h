#pragma once

#include "json/Json.h"

namespace web { namespace rest {

   //--------------------------------------   
   ///\brief   Class which role is to dispatch rest queries to appropriate code
   ///\note    Each rest method handler have to be registered with a pattern /device/*; with a fonction pointer to call in case of such a query
   ///         All the patterns are sorted so wilcard are low priority
   //-------------------------------------- 
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
      ///\brief Print content to log
      //--------------------------------------   
      void printContentToLog();

   private:
      //--------------------------------------   
      ///\brief   A container class for an url pattern with its function pointers
      /// /widget/*/acquisitions (=> widget, *, acquisitions)
      //-------------------------------------
      class CUrlPattern
      {
      public:
         //--------------------------------------   
         ///\brief   Constructor
         ///\param [in]    pattern           the url pattern : ie. : /widget/*/acquisitions
         ///\param [in]    functionPtr       the function pointer to call when url match configuration pattern
         ///\param [in]    indirectPtr       the function pointer which will call the functionPtr (indirect so some common process can be handled in this function)
         //-------------------------------------- 
         CUrlPattern(const std::vector<std::string> & pattern, CRestMethodHandler & handler, CRestMethodIndirector & indirector);

         //--------------------------------------   
         ///\brief   Destructor
         //--------------------------------------
         virtual ~CUrlPattern();

         //--------------------------------------   
         ///\brief   Get the pattern
         ///\return  the pattern (vector of string)
         //--------------------------------------
         const std::vector<std::string> & getPattern() const;

         //--------------------------------------   
         ///\brief   Get the function pointer to use when an url match the pattern
         ///\return  the function pointer
         //--------------------------------------
         const CRestMethodHandler & getMethodHandler() const;

         //--------------------------------------   
         ///\brief   Get the function pointer indirector (which encapsulate treatment for methodHandler)
         ///\return  the function pointer
         //--------------------------------------
         const CRestMethodIndirector & getMethodIndirector() const;

         //--------------------------------------   
         ///\brief   Operator <   Allow automatic sorting in std::map or std::set
         ///\param [in] right The object to compare to
         ///\return  true if current element is before "right", else other cases
         //--------------------------------------
         bool operator<(const CUrlPattern &right) const;

         //--------------------------------------   
         ///\brief   Utility method to get pattern as a string i.e. : /device/matchcapacity/*/*
         ///\return  the pattern as a string
         //--------------------------------------
         std::string toString() const;

      private:
         //--------------------------------------   
         ///\brief   The pattern
         //--------------------------------------
         std::vector<std::string> m_pattern;

         //--------------------------------------   
         ///\brief   The function pointer
         //--------------------------------------
         CRestMethodHandler m_methodHandler;

         //--------------------------------------   
         ///\brief   The indirector function pointer
         //--------------------------------------
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
      typedef std::set< boost::shared_ptr<CUrlPattern> > RestMethodMap;
      std::map<std::string, RestMethodMap > m_handledFunctions;
   };

} //namespace rest
} //namespace web 


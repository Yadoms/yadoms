#include "stdafx.h"
#include "RestDispatcher.h"
#include "json/JsonResult.h"
#include <shared/Log.h>

namespace web { namespace rest {

   CRestDispatcher::CRestDispatcher()
   {
   }

   CRestDispatcher::~CRestDispatcher()
   {
   }

   void CRestDispatcher::registerRestMethodHandler(const std::string & requestType, const CUrlPattern & configKeywords, CRestMethodHandler functionPtr, CRestMethodIndirector indirectPtr /*= NULL*/)
   {
      m_handledFunctions[requestType][configKeywords] = std::make_pair(functionPtr, indirectPtr);
   }



   web::rest::json::CJson CRestDispatcher::dispath(const std::string & requestType, const std::vector<std::string> & url, const web::rest::json::CJson & requestContent)
   {
      //check the te requestType has some functions
      if(m_handledFunctions.find(requestType) != m_handledFunctions.end())
      {
         //browse all patterns and check if it match to given url
         RestMethodMap::iterator iPatterns;

         //first pass without handling wildcards (priority to full written patterns)
         for(iPatterns = m_handledFunctions[requestType].begin(); iPatterns != m_handledFunctions[requestType].end(); ++iPatterns)
         {
            if(match(url, iPatterns->first, false))
               return callRealMethod(iPatterns->second.first, iPatterns->second.second, url, requestContent);
         }

         //second pass with handling wildcards
         for(iPatterns = m_handledFunctions[requestType].begin(); iPatterns != m_handledFunctions[requestType].end(); ++iPatterns)
         {
            if(match(url, iPatterns->first, true))
               return callRealMethod(iPatterns->second.first, iPatterns->second.second, url, requestContent);
         }
         return web::rest::json::CJsonResult::GenerateError("This REST url is not handled");
      }
      else
         return web::rest::json::CJsonResult::GenerateError("The type of request : " + requestType + " is not handled");
   }

   const bool CRestDispatcher::match(const std::vector<std::string> & url, const CUrlPattern & urlPattern, const bool allowWildcard)
   {
      if(url.size() == urlPattern.size())
      {
         for(unsigned int i=0; i<urlPattern.size(); i++)
         {
            if(!allowWildcard && urlPattern[i] == "*")
               return false;

            if(urlPattern[i] != "*" && !boost::iequals(urlPattern[i], url[i]))
            {
               return false;
            }
         }
         return true;
      }
      return false;
   }

   web::rest::json::CJson CRestDispatcher::callRealMethod(CRestMethodHandler realMethod, CRestMethodIndirector encapsulatedMethod, const std::vector<std::string> & url, const web::rest::json::CJson & requestContent)
   {
      if(encapsulatedMethod != NULL)
         return encapsulatedMethod(realMethod, url, requestContent);
      return realMethod(url, requestContent);
   }

} //namespace rest
} //namespace web 

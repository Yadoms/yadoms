#include "stdafx.h"
#include "RestDispatcher.h"
#include "Result.h"
#include <shared/Log.h>

namespace web { namespace rest {

   CRestDispatcher::CRestDispatcher()
   {
      
   }

   CRestDispatcher::~CRestDispatcher()
   {
   }

   void CRestDispatcher::registerRestMethodHandler(const std::string & requestType, const std::vector<std::string> & configKeywords, CRestMethodHandler functionPtr, CRestMethodIndirector indirectPtr /*= NULL*/)
   {
      m_handledFunctions[requestType].insert( CUrlPattern(configKeywords, functionPtr, indirectPtr) );
   }


   CRestDispatcher::CUrlPattern::CUrlPattern(const std::vector<std::string> & pattern, CRestMethodHandler & handler, CRestMethodIndirector & indirector)
      :m_pattern(pattern), m_methodHandler(handler), m_methodIndirector(indirector)
   {
   }

    CRestDispatcher::CUrlPattern::~CUrlPattern()
   {
   }

   const std::vector<std::string> &  CRestDispatcher::CUrlPattern::getPattern() const 
   { 
      return m_pattern; 
   }
   const CRestDispatcher::CRestMethodHandler &  CRestDispatcher::CUrlPattern::getMethodHandler() const 
   {
      return m_methodHandler; 
   }
   const CRestDispatcher::CRestMethodIndirector &  CRestDispatcher::CUrlPattern::getMethodIndirector() const 
   { 
      return m_methodIndirector; 
   }



   bool CRestDispatcher::CUrlPattern::operator<(const CUrlPattern &right) const
   {
      unsigned int minSize = std::min(getPattern().size(),right.getPattern().size());
      for(unsigned int i=0; i<minSize; i++)
      {

         if(getPattern()[i] != "*" && right.getPattern()[i] != "*")
         {
            //none of them have a wildcard, see next character
         } 
         else if(getPattern()[i] != "*" && right.getPattern()[i] == "*")
         {
            return true;
         }
         else if(getPattern()[i] == "*" && right.getPattern()[i] != "*")
         {
            return false;
         }
         else
         {
            //do nothing, see next step
         }
      }

      return (getPattern().size() < right.getPattern().size());
   }

   std::string CRestDispatcher::CUrlPattern::toString() const
   {
      std::string pattern;
      for(unsigned int i=0; i<getPattern().size(); ++i)
      {
         pattern+= "/";
         pattern+= getPattern()[i];
      }
      return pattern;
   }



   void CRestDispatcher::printContentToLog()
   {
      std::map<std::string, RestMethodMap>::iterator i;
      for(i= m_handledFunctions.begin(); i!=m_handledFunctions.end(); ++i)
      {
         YADOMS_LOG(debug) << "******************************************************";
         YADOMS_LOG(debug) << "Requests type : " << i->first;

         RestMethodMap::iterator iPatterns;
         for(iPatterns = i->second.begin(); iPatterns!= i->second.end(); ++iPatterns)
         {
            YADOMS_LOG(debug) << iPatterns->toString();
         }
      }
   }


   boost::shared_ptr<shared::serialization::IDataSerializable> CRestDispatcher::dispath(const std::string & requestType, const std::vector<std::string> & url, const std::string & requestContent)
   {
      //check that requestType has some functions
      if(m_handledFunctions.find(requestType) != m_handledFunctions.end())
      {
         //browse all patterns and check if it match to given url
         RestMethodMap::iterator iPatterns;

         for(iPatterns = m_handledFunctions[requestType].begin(); iPatterns != m_handledFunctions[requestType].end(); ++iPatterns)
         {
            if(match(url, *iPatterns))
            {
               return callRealMethod(iPatterns->getMethodHandler(), iPatterns->getMethodIndirector(), url, requestContent);
            }
         }

         return CResult::GenerateError("This REST url is not handled");
      }
      
      return CResult::GenerateError("The type of request : " + requestType + " is not handled");
   }

   bool CRestDispatcher::match(const std::vector<std::string> & url, const CUrlPattern & urlPattern)
   {
      if(url.size() == urlPattern.getPattern().size())
      {
         for(unsigned int i=0; i<urlPattern.getPattern().size(); i++)
         {
            if(urlPattern.getPattern()[i] != "*" && !boost::iequals(urlPattern.getPattern()[i], url[i]))
            {
               return false;
            }
         }
         return true;
      }
      return false;
   }

   boost::shared_ptr<shared::serialization::IDataSerializable> CRestDispatcher::callRealMethod(CRestMethodHandler realMethod, CRestMethodIndirector encapsulatedMethod,
      const std::vector<std::string> & url, const std::string & requestContent)
   {
      if(encapsulatedMethod != NULL)
         return encapsulatedMethod(realMethod, url, requestContent);
      return realMethod(url, requestContent);
   }

} //namespace rest
} //namespace web 

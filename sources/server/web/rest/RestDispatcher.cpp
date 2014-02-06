#include "stdafx.h"
#include "RestDispatcher.h"


CRestDispatcher::CRestDispatcher()
{
}

CRestDispatcher::~CRestDispatcher()
{
}

void CRestDispatcher::registerRestMethodHandler(const std::string & requestType, const CUrlPattern & configKeywords, CRestMethodHandler functionPtr)
{
   m_handledFunctions[requestType][configKeywords] = functionPtr;
}



CJson CRestDispatcher::dispath(const std::string & requestType, const std::vector<std::string> & url, const CJson & requestContent)
{
   //check the te requestType has some functions
   if(m_handledFunctions.find(requestType) != m_handledFunctions.end())
   {
      //browse all patterns and check if it match to given url
      std::map<CUrlPattern, CRestMethodHandler>::iterator iPatterns;
      for(iPatterns = m_handledFunctions[requestType].begin(); iPatterns != m_handledFunctions[requestType].end(); iPatterns++)
      {
         if(match(url, iPatterns->first))
            return iPatterns->second(url, requestContent);
      }
   }
   return CJson();
}

const bool CRestDispatcher::match(const std::vector<std::string> & url, const CUrlPattern & urlPattern)
{
   if(url.size() == urlPattern.size())
   {
      for(unsigned int i=0; i<urlPattern.size(); i++)
      {
         if(urlPattern[i] != "*" && !boost::iequals(urlPattern[i], url[i]))
         {
            return false;
         }
      }
      return true;
   }
   return false;
}
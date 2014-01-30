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



CJson CRestDispatcher::dispath(const std::string & requestType, const std::vector<std::string> & url)
{
   //check the te requestType has some functions
   if(m_handledFunctions.find(requestType) != m_handledFunctions.end())
   {
      //browse all patterns and check if it match to given url
      std::map<CUrlPattern, CRestMethodHandler>::iterator iPatterns;
      for(iPatterns = m_handledFunctions[requestType].begin(); iPatterns != m_handledFunctions[requestType].end(); iPatterns++)
      {
         if(match(url, iPatterns->first))
            return iPatterns->second(url);
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
         if(urlPattern[i] != "*" && strnicmp(urlPattern[i].c_str(), url[i].c_str(), urlPattern[i].size())!=0)
         {
            return false;
         }
      }
      return true;
   }
   return false;
}
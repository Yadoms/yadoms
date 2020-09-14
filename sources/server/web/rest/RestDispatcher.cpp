#include "stdafx.h"
#include "RestDispatcher.h"
#include "Result.h"
#include <shared/Log.h>

namespace web
{
   namespace rest
   {
      void CRestDispatcher::registerRestMethodHandler(const std::string& requestType,
                                                      const std::vector<std::string>& configKeywords,
                                                      CRestMethodHandler functionPtr,
                                                      CRestMethodIndirector indirectPtr /*= NULL*/)
      {
         m_handledFunctions[requestType].insert(CUrlPattern(configKeywords,
                                                            functionPtr,
                                                            indirectPtr));
      }


      CRestDispatcher::CUrlPattern::CUrlPattern(const std::vector<std::string>& pattern,
                                                CRestMethodHandler& handler,
                                                CRestMethodIndirector& indirector)
         : m_pattern(pattern),
           m_methodHandler(handler),
           m_methodIndirector(indirector)
      {
      }

      const std::vector<std::string>& CRestDispatcher::CUrlPattern::getPattern() const
      {
         return m_pattern;
      }

      const CRestDispatcher::CRestMethodHandler& CRestDispatcher::CUrlPattern::getMethodHandler() const
      {
         return m_methodHandler;
      }

      const CRestDispatcher::CRestMethodIndirector& CRestDispatcher::CUrlPattern::getMethodIndirector() const
      {
         return m_methodIndirector;
      }


      bool CRestDispatcher::CUrlPattern::operator<(const CUrlPattern& right) const
      {
         const auto minSize = std::min(getPattern().size(), right.getPattern().size());
         for (unsigned int i = 0; i < minSize; i++)
         {
            if (getPattern()[i] != "*" && right.getPattern()[i] != "*")
            {
               //none of them have a wildcard, see next character
            }
            else if (getPattern()[i] != "*" && right.getPattern()[i] == "*")
            {
               return true;
            }
            else if (getPattern()[i] == "*" && right.getPattern()[i] != "*")
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
         for (unsigned int i = 0; i < getPattern().size(); ++i)
         {
            pattern += "/";
            pattern += getPattern()[i];
         }
         return pattern;
      }


      void CRestDispatcher::printContentToLog()
      {
         for (auto iFunction = m_handledFunctions.begin(); iFunction != m_handledFunctions.end(); ++iFunction)
         {
            YADOMS_LOG(debug) << "******************************************************";
            YADOMS_LOG(debug) << "Requests type : " << iFunction->first;

            for (auto iPatterns = iFunction->second.begin(); iPatterns != iFunction->second.end(); ++iPatterns)
            {
               YADOMS_LOG(debug) << iPatterns->toString();
            }
         }
      }


      boost::shared_ptr<shared::serialization::IDataSerializable> CRestDispatcher::dispatch(
         const std::string& requestType,
         const std::vector<std::string>& url,
         const std::string& requestContent)
      {
         //check that requestType has some functions
         if (m_handledFunctions.find(requestType) != m_handledFunctions.end())
         {
            const auto& pattern = std::find_if(m_handledFunctions[requestType].begin(),
                                               m_handledFunctions[requestType].end(),
                                               [&url](const auto& iPatterns)
                                               {
                                                  return (match(url, iPatterns));
                                               });
            if (pattern != m_handledFunctions[requestType].end())
               return callRealMethod(pattern->getMethodHandler(),
                                     pattern->getMethodIndirector(),
                                     url,
                                     requestContent);

            throw std::invalid_argument("REST request url not handled");
         }

         throw std::invalid_argument("REST request type not handled");
      }

      bool CRestDispatcher::match(const std::vector<std::string>& url,
                                  const CUrlPattern& urlPattern)
      {
         if (urlPattern.getPattern().size() != url.size())
            return false;

         return std::equal(urlPattern.getPattern().begin(),
                           urlPattern.getPattern().end(),
                           url.begin(),
                           [](const auto& patternItem, const auto& urlItem)
                           {
                              return patternItem == "*" || boost::iequals(patternItem, urlItem);
                           });
      }

      boost::shared_ptr<shared::serialization::IDataSerializable> CRestDispatcher::callRealMethod(
         CRestMethodHandler realMethod,
         CRestMethodIndirector encapsulatedMethod,
         const std::vector<std::string>& url,
         const std::string& requestContent)
      {
         if (encapsulatedMethod != NULL)
            return encapsulatedMethod(realMethod, url, requestContent);
         return realMethod(url, requestContent);
      }
   } //namespace rest
} //namespace web 

#include "stdafx.h"
#include "Uri.h"
#include <shared/StringExtension.h>
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

namespace tools { namespace web { 

   CUri::CUri(const std::string & uri)
   {
      try
      {
         boost::regex re("^((http[s]?|ftp):\\/)?\\/?([^:\\/\\s]+)(:([^\\/]*))?((\\/\\w+)*\\/)([\\w\\-\\.]+[^#?\\s]+)(\\?([^#]*))?(#(.*))?$");
         boost::cmatch matches;
         m_isValid = boost::regex_match(uri.c_str(), matches, re);

         for (unsigned int i = 0; i < matches.size(); i++)
         {
            std::string match(matches[i].first, matches[i].second);
            m_resultFields.push_back(match);
         }
      }
      catch (boost::regex_error& e)
      {
         YADOMS_LOG(error) << "The regexp is invalid! : " << e.what();
         m_isValid = false;
      }

      if(!m_isValid)
         throw shared::exception::CException("The URI is not valid :" + uri);
   }

   CUri::~CUri()
   {
   }

   bool CUri::isValid() const
   {
      return m_isValid;
   }

   const std::string & CUri::getUriSchema() const
   {
      return getResult(kSchema);
   }

   const std::string & CUri::getUriDomain() const
   {
      return getResult(kDomain);
   }

   const std::string & CUri::getUriPort() const
   {
      return getResult(kPort);
   }

   const std::string & CUri::getUriPath() const
   {
      return getResult(kPath);
   }

   const std::string & CUri::getUriFile() const
   {
      return getResult(kFile);
   }
   const std::string & CUri::getUriQueryString() const
   {
      return getResult(kQuerystring);
   }

   const std::string & CUri::getUriHash() const
   {
      return getResult(kHash);
   }

   const std::string & CUri::getResult(EResultFieldOrder field) const
   {
      if(m_resultFields.size()>(unsigned int)field)
      {
         return m_resultFields[field];
      }
      return shared::CStringExtension::EmptyString;
   }

} //namespace web
} //namespace tools

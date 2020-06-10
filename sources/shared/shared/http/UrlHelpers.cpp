#include "stdafx.h"
#include "UrlHelpers.h"
#include <Poco/URI.h>

namespace shared
{
   namespace http
   {
      void CUrlHelpers::appendPath(std::string& url, const std::string& pathToAppend)
      {
         if (!boost::iends_with(url, "/"))
            url += "/";
         url += pathToAppend;
      }

      std::string CUrlHelpers::getFileName(const std::string& url)
      {
         auto pocoUri = Poco::URI(url);
         std::vector<std::string> segments;
         pocoUri.getPathSegments(segments);
         if (!segments.empty())
         {
            auto& maybeFile = segments.back();
            if (boost::icontains(maybeFile, "."))
               return maybeFile;
         }
         return std::string();
      }
   } //namespace http
} //namespace shared

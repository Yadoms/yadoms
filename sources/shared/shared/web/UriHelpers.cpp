#include "stdafx.h"
#include "UriHelpers.h"

namespace shared { namespace web {

   void CUriHelpers::appendPath(Poco::URI & uri, const std::string & pathToAppend)
   {
      std::string initialPath = uri.getPath();
      if (!boost::iends_with(initialPath, "/"))
         initialPath += "/";
      initialPath += pathToAppend;
      uri.setPath(initialPath);
   }  
   
   
   std::string CUriHelpers::getFile(Poco::URI & uri)
   {
      std::vector<std::string> segments;
      uri.getPathSegments(segments);
      if (!segments.empty())
      {
         std::string & maybeFile = segments.back();
         if (boost::icontains(maybeFile, "."))
            return maybeFile;
      }
      return std::string();
   }




} //namespace web
} //namespace shared
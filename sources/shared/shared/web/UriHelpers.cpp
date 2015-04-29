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


} //namespace web
} //namespace shared
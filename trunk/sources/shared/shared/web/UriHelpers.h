#pragma once

#include <shared/Export.h>
#include <Poco/URI.h>

namespace shared { namespace web { 

   //---------------------------------
   ///\brief Utility (static) class which simplify use of Poco::URI
   //---------------------------------
   class YADOMS_SHARED_EXPORT CUriHelpers
   {
   public:
      //---------------------------------
      ///\brief Append a path to an existing URI
      ///\param [in] uri            The URI to update
      ///\param [in] pathToAppend   The path part to append
      //---------------------------------
      static void appendPath(Poco::URI & uri, const std::string & pathToAppend);
   };

} //namespace web
} //namespace shared

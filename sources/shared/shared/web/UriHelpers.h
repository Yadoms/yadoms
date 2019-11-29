#pragma once

#include <Poco/URI.h>

namespace shared { namespace web { 

   //---------------------------------
   ///\brief Utility (static) class which simplify use of Poco::URI
   //---------------------------------
   class CUriHelpers
   {
   public:
      //---------------------------------
      ///\brief Append a path to an existing URI
      ///\param [in] uri            The URI to update
      ///\param [in] pathToAppend   The path part to append
      //---------------------------------
      static void appendPath(Poco::URI & uri, const std::string & pathToAppend);

      //---------------------------------
      ///\brief Get the filename from an URI
      ///\param [in] uri            The URI 
      ///\return The filename if found
      ///
      /// example:
      ///   "http://www.yadoms.com/download/package.zip" -> "package.zip"
      ///   "http://www.yadoms.com/download" -> ""
      //---------------------------------
      static std::string getFile(Poco::URI & uri);
   };

} //namespace web
} //namespace shared

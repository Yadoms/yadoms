#pragma once

namespace shared
{
   namespace web
   {
      //---------------------------------
      ///\brief Helpers class around url management
      //---------------------------------
      class CUrlHelpers
      {
      public:
         //---------------------------------
         ///\brief Append a path to an existing URL
         ///\param [in] url            The URL to update
         ///\param [in] pathToAppend   The path part to append
         //---------------------------------
         static void appendPath(std::string& url,
                                const std::string& pathToAppend);

         //---------------------------------
         ///\brief Get the filename from an URL
         ///\param [in] url            The URL 
         ///\return The filename if found
         ///
         /// example:
         ///   "http://www.yadoms.com/download/package.zip" -> "package.zip"
         ///   "http://www.yadoms.com/download" -> ""
         //---------------------------------
         static std::string getFileName(const std::string& url);
      };
   } //namespace web
} //namespace shared

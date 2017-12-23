#pragma once

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace web { namespace poco {

  
   //-------------------------------------
   ///\brief Class which manage cache with browsers
   //-------------------------------------
   class CWebsiteCacheManager
   {
   public:
      //-------------------------------------
      ///\brief Constructor
      //-------------------------------------
      CWebsiteCacheManager();

      //-------------------------------------
      ///\brief Destructor
      //-------------------------------------
      virtual ~CWebsiteCacheManager();

      //-------------------------------------
      ///\brief The check cache result
      //-------------------------------------
      typedef enum
      {
         //-------------------------------------
         ///\brief The file is to send to browser
         //-------------------------------------
         kSendFile,

         //-------------------------------------
         ///\brief The file did not changed compare to browser request
         //-------------------------------------
         kNothingChanged
      }E_CacheResult;
      
      //-------------------------------------
      ///\brief Check the cache
      ///\param [in] fullPath The full path
      ///\param [in] request  The http request
      ///\param [in] response The http response
      ///\return The cache result. all headers have been added to response.
      //-------------------------------------
      E_CacheResult checkCache(const std::string & fullPath, Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) const;

   private:
      //-------------------------------------
      ///\brief Check if a file is excluded of cache system
      ///\param [in] fullPath The full path
      ///\return true if dile is excluded (no cache) or false (cache is possible)
      //-------------------------------------
      bool isFileExcluded(const std::string & fullPath) const;

      //-------------------------------------
      ///\brief Get the ETag from the request
      ///\param [in] request  The http request
      ///\return The ETag found in headers; empty if not found
      //-------------------------------------
      std::string getETagFromRequest(Poco::Net::HTTPServerRequest& request) const;

      //-------------------------------------
      ///\brief Get the ETag from the file
      ///\param [in] fullPath  The file path
      ///\return The ETag computed
      //-------------------------------------
      std::string generateETag(const std::string & fullPath) const;

      //-------------------------------------
      ///\brief Add HTTP response header when cache is disabled
      ///\param [in] response  The http response
      //-------------------------------------
      void addResponseHeaderCacheDisabled(Poco::Net::HTTPServerResponse& response) const;

      //-------------------------------------
      ///\brief Add HTTP response header when the file is excluded
      ///\param [in] response  The http response
      //-------------------------------------
      void addResponseHeaderFileExcluded(Poco::Net::HTTPServerResponse& response) const;

      //-------------------------------------
      ///\brief Add HTTP response header when cache is enabled
      ///\param [in] response  The http response
      //-------------------------------------
      void addResponseHeaderCacheEnabled(Poco::Net::HTTPServerResponse& response, const std::string & etag) const;

      //-------------------------------------
      ///\brief Indicate if cache feature is disabled
      //-------------------------------------
      bool m_cacheDisabled;

      //-------------------------------------
      ///\brief Contains the patterns to exclude from caching process
      //-------------------------------------
      std::vector<std::string> m_excluePattern;

      //-------------------------------------
      ///\brief The cache mode (public, private)
      //-------------------------------------
      const std::string m_cacheMode;

      //-------------------------------------
      ///\brief The max-age of the cache (browser side), in seconds
      //-------------------------------------
      const int m_cacheMaxAge;
   };



} //namespace poco
} //namespace web


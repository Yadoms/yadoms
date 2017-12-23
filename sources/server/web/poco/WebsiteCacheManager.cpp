#include "stdafx.h"
#include "WebsiteCacheManager.h"
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPBasicCredentials.h>
#include "MimeType.h"
#include <shared/encryption/Md5.h>
#include <shared/ServiceLocator.h>
#include "startupOptions/IStartupOptions.h"

namespace web { namespace poco {

      CWebsiteCacheManager::CWebsiteCacheManager()
         :m_cacheMode("private"), m_cacheMaxAge(120)
      {
         //retrieve startup options
         auto startupOptions = shared::CServiceLocator::instance().get<const startupOptions::IStartupOptions>();
         m_cacheDisabled = startupOptions->getNoWebServerCacheFlag();
      }

      CWebsiteCacheManager::~CWebsiteCacheManager()
      {

      }

      bool CWebsiteCacheManager::isFileExcluded(const std::string & fullPath) const
      {
         for (auto i = m_excluePattern.begin(); i != m_excluePattern.end(); ++i)
         {
            if (boost::iends_with(fullPath, *i))
               return true;
         }
         return false;
      }

      std::string CWebsiteCacheManager::getETagFromRequest(Poco::Net::HTTPServerRequest& request) const
      {
         //The ETag is found for header "If-None-Match"
         return request.get("If-None-Match", ""); //second parameter is default value
      }

      std::string CWebsiteCacheManager::generateETag(const std::string & fullPath) const
      {
         return shared::encryption::CMd5::digestFile(fullPath);
      }

      void CWebsiteCacheManager::addResponseHeaderCacheDisabled(Poco::Net::HTTPServerResponse& response) const
      {
         response.set("Cache-Control", "no-cache, no-store, must-revalidate");
      }

      void CWebsiteCacheManager::addResponseHeaderFileExcluded(Poco::Net::HTTPServerResponse& response) const
      {
         response.set("Cache-Control", "no-cache");
      }

      void CWebsiteCacheManager::addResponseHeaderCacheEnabled(Poco::Net::HTTPServerResponse& response, const std::string & etag) const
      {
         response.set("Cache-Control", (boost::format("%1%, max-age=%2%") % m_cacheMode % m_cacheMaxAge).str() ); //2 minutes
         response.set("ETag", etag);
      }


      CWebsiteCacheManager::E_CacheResult CWebsiteCacheManager::checkCache(const std::string & fullPath, Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) const
      {
         if (m_cacheDisabled)
         {
            //disable cache, force reload
            addResponseHeaderCacheDisabled(response);
         }
         else
         {
            if (isFileExcluded(fullPath))
            {
               addResponseHeaderFileExcluded(response);
            }
            else
            {
               std::string etag = generateETag(fullPath);
               addResponseHeaderCacheEnabled(response, etag);

               //if server have file in cache and has expired, just check if etag has changed
               std::string etagFromRequest = getETagFromRequest(request);
               if (!etagFromRequest.empty() && boost::iequals(etagFromRequest, etag))
               {
                  return kNothingChanged;
               }
               else
               {
                  //the file has changed, so send it a new time
               }
            }
         }
         return kSendFile;
      }

} //namespace poco
} //namespace web


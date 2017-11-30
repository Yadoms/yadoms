#pragma once

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include "WebsiteCacheManager.h"

namespace web { namespace poco {

  
   class CWebsiteRequestHandler : public Poco::Net::HTTPRequestHandler
   {
   public:
      //-------------------------------------
      ///\brief Constructor
      ///\param [in]    documentRoot   The path to website files
      ///\param [in]    alias          The map of alias <alias, path>
      //-------------------------------------
      CWebsiteRequestHandler(const std::string & documentRoot, const std::map<std::string, std::string> & alias);

      //-------------------------------------
      ///\brief Destructor
      //-------------------------------------
      virtual ~CWebsiteRequestHandler();

      //-------------------------------------
      ///\brief Configure an alias
      ///\param [in]    alias             The alias name used in url 
      ///\param [in]    documentsPath     The path of files for this alias
      //-------------------------------------
      void configureAlias(const std::string & alias, const std::string & documentsPath);

      // Poco::Net::HTTPRequestHandler implementation
      void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
      // [END] Poco::Net::HTTPRequestHandler implementation

   private:
      //-------------------------------------
      ///\brief Read and send file as request answer
      ///\param [in]    fullpath          The file path
      ///\param [in]    request           The http request
      ///\param [in]    response          The http response
      ///\return true if file is found and sent, else false
      //-------------------------------------
      bool readAndSendFile(std::string & fullpath, Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

      //-------------------------------------
      ///\brief The base files path
      //-------------------------------------
      std::string m_documentRoot;
      
      //-------------------------------------
      ///\brief The aliases (alis, path to file)
      //-------------------------------------
      std::map<std::string, std::string> m_alias;
      
      //-------------------------------------
      ///\brief The cache manager
      //-------------------------------------
      CWebsiteCacheManager m_cacheManager;
   };



} //namespace poco
} //namespace web


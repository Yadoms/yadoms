#pragma once

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace web { namespace poco {

  
   class CWebsiteRequestHandler : public Poco::Net::HTTPRequestHandler
   {
   public:
      //-------------------------------------
      ///\brief Constructor
      ///\param [in]    documentRoot   The path to website files
      //-------------------------------------
      CWebsiteRequestHandler(const std::string & documentRoot);

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
      ///\brief The base files path
      //-------------------------------------
      std::string m_documentRoot;
      
      //-------------------------------------
      ///\brief The aliases (alis, path to file)
      //-------------------------------------
      std::map<std::string, std::string> m_alias;
   };



} //namespace poco
} //namespace web


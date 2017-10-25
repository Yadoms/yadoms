#include "stdafx.h"
#include "WebsiteRequestHandler.h"
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPBasicCredentials.h>
#include "MimeType.h"
#include <shared/encryption/Md5.h>
#include <shared/ServiceLocator.h>
#include "startupOptions/IStartupOptions.h"

namespace web { namespace poco {

      CWebsiteRequestHandler::CWebsiteRequestHandler(const std::string & documentRoot, const std::map<std::string, std::string> & alias)
         :m_documentRoot(documentRoot)
      {
         //retrieve startup options
         auto startupOptions = shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>();
         m_cacheDisabled = startupOptions->getNoWebServerCacheFlag();

         std::map<std::string, std::string>::const_iterator i;
         for (i = alias.begin(); i != alias.end(); ++i)
            configureAlias(i->first, i->second);
      }

      CWebsiteRequestHandler::~CWebsiteRequestHandler()
      {

      }

      void CWebsiteRequestHandler::configureAlias(const std::string & alias, const std::string & documentsPath)
      {
         if (boost::filesystem::is_directory(boost::filesystem::path(documentsPath)))
         {
            std::string realAlias = alias;
            if (!boost::istarts_with(realAlias, "/"))
               realAlias = "/" + realAlias;
            if (!boost::iends_with(realAlias, "/"))
               realAlias = realAlias + "/";

            std::string realDocumentsPath = documentsPath;
            if (!boost::iends_with(realDocumentsPath, "/"))
               realDocumentsPath = realDocumentsPath + "/";

            m_alias[realAlias] = realDocumentsPath;
         }
         else 
         {
            std::string realAlias = alias;
            if (!boost::istarts_with(realAlias, "/"))
               realAlias = "/" + realAlias;
            m_alias[realAlias] = documentsPath;
         }
      }

      bool CWebsiteRequestHandler::readAndSendFile(std::string & fullpath, Poco::Net::HTTPServerResponse& response)
      {
         // Determine the file extension.
         std::size_t last_slash_pos = fullpath.find_last_of("/");
         std::size_t last_dot_pos = fullpath.find_last_of(".");
         std::string extension;
         if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
         {
            extension = fullpath.substr(last_dot_pos + 1);
         }

         if (m_cacheDisabled)
         {
            //disable cache, force reload
            response.set("Cache-Control", "no-cache, no-store, must-revalidate");
         }
         else
         {
            //allow browser to cache ressource
            // public : allow any intermediate proxies (not only the browser)
            // max-age : keep in memory for two months
            // etag : the md5 hash of each file. Computed at each requests, it ensure that the file hasn't changed. This is a browser behavior (etag change detection)
            response.set("Cache-Control", "private, max-age=5259477"); //2 months
            response.set("ETag", shared::encryption::CMd5::digestFile(fullpath));
         }

         std::ifstream is(fullpath.c_str(), std::ios::in | std::ios::binary);
         if (is)
         {
            response.setChunkedTransferEncoding(true);
            response.setContentType(CMimeType::extension_to_type(extension));
            std::ostream& ostr = response.send();
            char buf[512];
            while (is.read(buf, sizeof(buf)).gcount() > 0)
               ostr.write(buf, (unsigned int)is.gcount());
            return true;
         }
         return false;
      }

      void CWebsiteRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
      {
         std::string uri = request.getURI();

         //extract the real uri (without url parameters, : index.html?param=value....
         std::size_t last_interrogation_pos = uri.find_last_of("?");
         if (last_interrogation_pos != std::string::npos)
            uri = uri.substr(0, last_interrogation_pos);

         // If path ends in slash (i.e. is a directory) then add "index.html".
         if (uri[uri.size() - 1] == '/')
         {
            uri += "index.html";
         }

         // Open the file to send back.
         std::string full_path = m_documentRoot + uri;

         std::map<std::string, std::string>::iterator i;
         for (i = m_alias.begin(); i != m_alias.end(); ++i)
         {
            if (boost::istarts_with(uri, i->first))
            {
               std::string repl = uri;
               repl.replace(0, i->first.size(), i->second);
               
               full_path = repl;
            }
         }
         
         //try to send file
         if (!readAndSendFile(full_path, response))
         {
            //the 404.html has not been found, just return a 404 hard coded message
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);

            std::string html404 = m_documentRoot + "/404.html";
            if (!readAndSendFile(html404, response))
            {
               //the file is not found, try to send 404.html file
               response.setContentType("text/html");
               response.setChunkedTransferEncoding(false);

               std::stringstream resp_body;
               resp_body << "<html><head><title>404 Not Found</title>";
               resp_body << "</head>";
               resp_body << "<body><p style=\"font-size: 24;\">Yadoms Web Service</p>";
               resp_body << "<p style=\"font-size: 24;\"><H1>Not Found</H1></p><p style=\"font-size: 24;\">";
               resp_body << "The requested URL " << request.getURI() << " was not found.";
               resp_body << "</p></body></html>";
               response.setContentLength(resp_body.str().length());
               std::ostream& ostr = response.send();
               ostr << resp_body.str();
            }
         }
         
      }
} //namespace poco
} //namespace web


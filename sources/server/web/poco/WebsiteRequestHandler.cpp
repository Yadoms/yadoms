#include "stdafx.h"
#include "WebsiteRequestHandler.h"
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include "MimeType.h"

namespace web { namespace poco {

      CWebsiteRequestHandler::CWebsiteRequestHandler(const std::string & documentRoot)
         :m_documentRoot(documentRoot)
      {

      }

      void CWebsiteRequestHandler::configureAlias(const std::string & alias, const std::string & documentsPath)
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


         // Determine the file extension.
         std::size_t last_slash_pos = uri.find_last_of("/");
         std::size_t last_dot_pos = uri.find_last_of(".");
         std::string extension;
         if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
         {
            extension = uri.substr(last_dot_pos + 1);
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
         
         std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
         if (is)
         {
            response.setChunkedTransferEncoding(true);
            response.setContentType(CMimeType::extension_to_type(extension));
            std::ostream& ostr = response.send();
            char buf[512];
            while (is.read(buf, sizeof(buf)).gcount() > 0)
               ostr.write(buf, (unsigned int)is.gcount());
         }
         else
         {
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
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
} //namespace poco
} //namespace web


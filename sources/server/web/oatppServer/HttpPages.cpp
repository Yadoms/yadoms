#include "stdafx.h"
#include "HttpPages.h"
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <utility>
#include <shared/Log.h>

namespace web
{
   namespace oatppServer
   {
      const std::map<std::string, std::string> CHttpPages::MimeTypes =
      {
         {".html", "text/html"},
         {".json", "text/json"},
         {".jpg", "image/jpeg"},
         {".jpeg", "image/jpeg"},
         {".png", "image/png"},
         {".gif", "image/gif"},
         {".ico", "image/x-icon"},
         {".css", "text/css"},
         {".js", "text/javascript"},
         {".xml", "text/xml"},
         {".bmp", "image/bmp"},
         {".pdf", "application/pdf"},
         {".wav", "audio/x-wav"},
         {".wave", "audio/x-wav"},
         {".svg", "image/svg+xml"},
         {".ttf", "font/ttf"}
      };

      CHttpPages::CHttpPages(boost::filesystem::path siteLocation)
         : m_siteLocation(std::move(siteLocation))
      {
      }

      std::shared_ptr<oatpp::web::server::HttpRequestHandler::OutgoingResponse> CHttpPages::handle(const std::shared_ptr<IncomingRequest>& request)
      {
         auto page = request->getStartingLine().path.std_str();
         if (page == "/")
            page = "/index.html";
         const auto fullPath = m_siteLocation / page;

         const auto buffer = oatpp::base::StrBuffer::loadFromFile(fullPath.string().c_str());
         if (buffer == nullptr)
         {
            YADOMS_LOG(error) << "The resource \' " << page << "\' was not found";
            return ResponseFactory::createResponse(Status::CODE_404,
                                                   getDefinedPage("The requested resource was not found!").c_str());
         }

         auto response = ResponseFactory::createResponse(Status::CODE_200,
                                                         oatpp::String(buffer));
         response->putHeader(HeaderContentType,
                             getMimeTypeFromPath(page));
         return response;
      }

      const char* CHttpPages::getMimeTypeFromPath(const std::string& path) const
      {
         const auto find = MimeTypes.find(extension(boost::filesystem::path(path)));
         if (find == MimeTypes.end())
         {
            YADOMS_LOG(error) << "Mimetype not supported for page \'" << path << "\'";
            return DefaultMimetype;
         }
         return find->second.c_str();
      }

      std::string CHttpPages::getDefinedPage(const std::string& text)
      //TODO la page 404 (ou autre codes d'erreur) ne devrait-elle pas être gérée par le front ?
      {
         std::stringstream ss;
         ss << "<html>";
         ss << "<head>";
         ss << "<title>TVM</title>";
         ss << "</head>";
         ss << "<body>";
         ss << "<h1>";
         ss << text;
         ss << "</h1>";
         ss << "</body>";
         ss << "</html>";
         return ss.str();
      }
   } //namespace oatppServer
} //namespace web

#include "stdafx.h"
#include "HttpPages.h"
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
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

      CHttpPages::CHttpPages(boost::filesystem::path siteLocation,
                             boost::shared_ptr<IAuthentication> authentication)
         : m_siteLocation(std::move(siteLocation)),
           m_authentication(std::move(authentication))
      {
      }

      std::shared_ptr<oatpp::web::server::HttpRequestHandler::OutgoingResponse> CHttpPages::handle(const std::shared_ptr<IncomingRequest>& request)
      {
         m_authentication->authenticate(request);

         auto page = request->getStartingLine().path.std_str();
         if (page == "/")
            page = "/index.html";
         const auto fullPath = m_siteLocation / page;

         const auto buffer = loadFile(fullPath);
         if (buffer.empty())
         {
            YADOMS_LOG(error) << "The resource \' " << page << "\' was not found";
            return ResponseFactory::createResponse(Status::CODE_404, oatpp::String(page.c_str()));
         }

         auto response = ResponseFactory::createResponse(Status::CODE_200,
                                                         oatpp::String(buffer));
         response->putHeader(HeaderContentType,
                             getMimeTypeFromPath(page));
         return response;
      }

      std::string CHttpPages::loadFile(const boost::filesystem::path& file)
      {
         std::ifstream stream(file.string());
         std::string str;

         stream.seekg(0, std::ios::end);
         // ReSharper disable once CppRedundantCastExpression
         str.reserve(static_cast<size_t>(stream.tellg()));
         stream.seekg(0, std::ios::beg);

         str.assign((std::istreambuf_iterator<char>(stream)),
                    std::istreambuf_iterator<char>());

         return str;
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
   } //namespace oatppServer
} //namespace web

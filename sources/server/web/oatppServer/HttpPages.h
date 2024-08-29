#pragma once
#include <oatpp/web/server/HttpRequestHandler.hpp>

#include "IAuthentication.h"

namespace web
{
   namespace oatppServer
   {
      //
      //\brief The HTTP page request handler
      //
      class CHttpPages final : public oatpp::web::server::HttpRequestHandler
      {
      public:
         CHttpPages(boost::filesystem::path siteLocation,
                    boost::shared_ptr<IAuthentication> authentication);
         ~CHttpPages() override = default;

         // oatpp::web::server::HttpRequestHandler Implementation
         std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override;
         // [END] oatpp::web::server::HttpRequestHandler Implementation

      private:
         static std::string loadFile(const boost::filesystem::path& file);
         static const char* getMimeTypeFromPath(const std::string& path);


         //TODO : voir code DAT (si utile. A voir selon les perfs du client Angular)
         //void AddFileToCache(const std::string& filePath);
         //void ClearCache();
         //void AddVirtualPage(const std::string& page, const std::string& destination);
         //void ClearVirtualPages();

         static const std::map<std::string, std::string> MimeTypes;
         static constexpr auto HeaderContentType = "Content-Type";
         static constexpr auto DefaultMimetype = "application/octet-stream";

         const boost::filesystem::path m_siteLocation;
         boost::shared_ptr<IAuthentication> m_authentication;
      };
   } //namespace oatppServer
} //namespace web

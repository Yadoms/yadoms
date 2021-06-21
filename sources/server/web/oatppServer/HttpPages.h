#pragma once
#include <oatpp/network/Server.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>

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
         CHttpPages(std::string name,
            boost::filesystem::path siteLocation);
         ~CHttpPages() override = default;

         // oatpp::web::server::HttpRequestHandler Implementation
         std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override;
         // [END] oatpp::web::server::HttpRequestHandler Implementation

      private:
         const char* getMimeTypeFromPath(const std::string& path) const;
         static std::string getDefinedPage(const std::string& text);


         //TODO : voir code DAT
         //void AddFileToCache(const std::string& filePath);
         //void ClearCache();
         //void AddVirtualPage(const std::string& page, const std::string& destination);
         //void ClearVirtualPages();

         static const std::map<std::string, std::string> MimeTypes;
         static constexpr auto HeaderContentType = "Content-Type";
         static constexpr auto DefaultMimetype = "application/octet-stream";

         const std::string m_name; //TODO utile ?
         const boost::filesystem::path m_siteLocation;
      };
   } //namespace oatppServer
} //namespace web

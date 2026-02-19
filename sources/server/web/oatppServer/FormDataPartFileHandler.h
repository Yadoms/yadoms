#pragma once
#include "IOatppFormDataPartHandler.h"
#include "web/rest/IFormDataPartFileHandler.h"

namespace web::oatpp_server
{
   class CFormDataPartFileHandler final : public IOatppFormDataPartHandler, public rest::IFormDataPartFileHandler
   {
   public:
      explicit CFormDataPartFileHandler(const boost::filesystem::path& into);
      ~CFormDataPartFileHandler() override = default;

      // IFormDataPartFileHandler Implementation
      long long fileSize() const override;
      std::string fileName() const override;
      // [END] IFormDataPartFileHandler Implementation

      // IFormDataPartHandler Implementation
      rest::EContentType contentType() const override;
      // [END] IFormDataPartHandler Implementation

      // IOatppFormDataPartHandler Implementation
      std::shared_ptr<oatpp::web::mime::multipart::PartReader> partReader() override;
      void setPart(std::shared_ptr<oatpp::web::mime::multipart::Part> part) override;
      // [END] IOatppFormDataPartHandler Implementation

   private:
      const std::shared_ptr<oatpp::web::mime::multipart::PartReader> m_reader;
      std::shared_ptr<oatpp::web::mime::multipart::Part> m_part;
   };
}

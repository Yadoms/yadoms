#pragma once
#include "IOatppFormDataPartHandler.h"
#include "web/rest/IFormDataPartStringHandler.h"

namespace web
{
   namespace oatppServer
   {
      class CFormDataPartStringHandler final : public IOatppFormDataPartHandler, public rest::IFormDataPartStringHandler
      {
      public:
         explicit CFormDataPartStringHandler();
         ~CFormDataPartStringHandler() override = default;

         // IFormDataPartStringHandler Implementation
         std::string string() const override;
         // [END] IFormDataPartStringHandler Implementation

         // IFormDataPartStringHandler Implementation
         rest::EContentType contentType() const override;
         // [END] IFormDataPartStringHandler Implementation

         // IOatppFormDataPartHandler Implementation
         std::shared_ptr<oatpp::web::mime::multipart::PartReader> partReader() override;
         void setPart(std::shared_ptr<oatpp::web::mime::multipart::Part> part) override;
         // [END] IOatppFormDataPartHandler Implementation

      private:
         const std::shared_ptr<oatpp::web::mime::multipart::PartReader> m_reader;
         std::shared_ptr<oatpp::web::mime::multipart::Part> m_part;
      };
   } //namespace oatppServer
} //namespace web

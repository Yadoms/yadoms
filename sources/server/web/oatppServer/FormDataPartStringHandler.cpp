#include "stdafx.h"
#include "FormDataPartStringHandler.h"
#include <oatpp/web/mime/multipart/InMemoryPartReader.hpp>
#include <oatpp/web/server/handler/ErrorHandler.hpp>

namespace web
{
   namespace oatppServer
   {
      CFormDataPartStringHandler::CFormDataPartStringHandler()
         : m_reader(oatpp::web::mime::multipart::createInMemoryPartReader())
      {
      }

      std::string CFormDataPartStringHandler::string() const
      {
         if (!m_part)
            return std::string();
         return m_part->getInMemoryData()->c_str();
      }

      rest::EContentType CFormDataPartStringHandler::contentType() const
      {
         if (!m_part)
            return rest::EContentType::kNone;
         return rest::ToContentType(m_part->getHeader(oatpp::web::protocol::http::Header::CONTENT_TYPE)->std_str());
      }

      std::shared_ptr<oatpp::web::mime::multipart::PartReader> CFormDataPartStringHandler::partReader()
      {
         return m_reader;
      }

      void CFormDataPartStringHandler::setPart(std::shared_ptr<oatpp::web::mime::multipart::Part> part)
      {
         m_part = part;
      }
   } //namespace oatppServer
} //namespace web

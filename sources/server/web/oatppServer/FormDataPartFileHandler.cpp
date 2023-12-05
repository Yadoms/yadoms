#include "stdafx.h"
#include "FormDataPartFileHandler.h"
#include <oatpp/web/mime/multipart/FileProvider.hpp>
#include <oatpp/web/server/handler/ErrorHandler.hpp>

namespace web
{
   namespace oatppServer
   {
      CFormDataPartFileHandler::CFormDataPartFileHandler(const boost::filesystem::path& into)
         : m_reader(oatpp::web::mime::multipart::createFilePartReader(into.string().c_str()))
      {
      }

      long long CFormDataPartFileHandler::fileSize() const
      {
         if (!m_part)
            return -1;
         return m_part->getPayload()->getKnownSize();
      }

      std::string CFormDataPartFileHandler::fileName() const
      {
         if (!m_part)
            return std::string();
         return m_part->getFilename();
      }

      rest::EContentType CFormDataPartFileHandler::contentType() const
      {
         if (!m_part)
            return rest::EContentType::kNone;
         return rest::ToContentType(m_part->getHeader(oatpp::web::protocol::http::Header::CONTENT_TYPE));
      }

      std::shared_ptr<oatpp::web::mime::multipart::PartReader> CFormDataPartFileHandler::partReader()
      {
         return m_reader;
      }

      void CFormDataPartFileHandler::setPart(std::shared_ptr<oatpp::web::mime::multipart::Part> part)
      {
         m_part = part;
      }
   } //namespace oatppServer
} //namespace web

#include "stdafx.h"
#include "FormDataPartStringHandler.h"
#include <oatpp/web/mime/multipart/InMemoryDataProvider.hpp>
#include <oatpp/web/server/handler/ErrorHandler.hpp>

using namespace web::oatpp_server;

CFormDataPartStringHandler::CFormDataPartStringHandler()
   : m_reader(oatpp::web::mime::multipart::createInMemoryPartReader(-1))
{
}

std::string CFormDataPartStringHandler::string() const
{
   if (!m_part)
      return std::string();
   return m_part->getPayload()->getInMemoryData();
}

web::rest::EContentType CFormDataPartStringHandler::contentType() const
{
   if (!m_part)
      return rest::EContentType::kNone;
   return rest::ToContentType(m_part->getHeader(oatpp::web::protocol::http::Header::CONTENT_TYPE));
}

std::shared_ptr<oatpp::web::mime::multipart::PartReader> CFormDataPartStringHandler::partReader()
{
   return m_reader;
}

void CFormDataPartStringHandler::setPart(std::shared_ptr<oatpp::web::mime::multipart::Part> part)
{
   m_part = part;
}

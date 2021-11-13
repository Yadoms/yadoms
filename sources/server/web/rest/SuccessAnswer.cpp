#include "stdafx.h"
#include "SuccessAnswer.h"

namespace web
{
   namespace rest
   {
      CSuccessAnswer::CSuccessAnswer(const shared::CDataContainer& result)
         : m_code(shared::http::ECodes::kOK),
           m_body(result.serialize()),
           m_contentType(EContentType::kJson)
      {
      }

      CSuccessAnswer::CSuccessAnswer(std::string result,
                                     const EContentType& contentType)
         : m_code(shared::http::ECodes::kOK),
           m_body(std::move(result)),
           m_contentType(contentType)
      {
      }

      shared::http::ECodes CSuccessAnswer::code() const
      {
         return m_code;
      }

      std::string CSuccessAnswer::body() const
      {
         return m_body;
      }

      EContentType CSuccessAnswer::contentType() const
      {
         return m_contentType;
      }

      std::shared_ptr<std::map<std::string, std::string>> CSuccessAnswer::customHeaders() const
      {
         return nullptr;
      }
   } //namespace rest
} //namespace web 

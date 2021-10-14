#include "stdafx.h"
#include "SuccessAnswer.h"

namespace web
{
   namespace rest
   {
      CSuccessAnswer::CSuccessAnswer()
         : m_code(shared::http::ECodes::kNoContent),
           m_contentType(EContentType::kNone)
      {
      }

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
   } //namespace rest
} //namespace web 

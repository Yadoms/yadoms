#include "stdafx.h"
#include "ErrorAnswer.h"

namespace web
{
   namespace rest
   {
      CErrorAnswer::CErrorAnswer(const shared::http::ECodes& code,
                                 std::string message)
         : m_code(code),
           m_body(std::move(message))
      {
      }

      shared::http::ECodes CErrorAnswer::code() const
      {
         return m_code;
      }

      std::string CErrorAnswer::body() const
      {
         return m_body;
      }

      EContentType CErrorAnswer::contentType() const
      {
         return EContentType::kPlainText;
      }
   } //namespace rest
} //namespace web 

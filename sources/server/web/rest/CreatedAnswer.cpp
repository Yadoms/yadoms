#include "stdafx.h"
#include "CreatedAnswer.h"

namespace web
{
   namespace rest
   {
      CCreatedAnswer::CCreatedAnswer(const std::string& resourceCreatedPath)
      {
         (*m_customHeaders)["Location"] = resourceCreatedPath;
      }

      shared::http::ECodes CCreatedAnswer::code() const
      {
         return shared::http::ECodes::kCreated;
      }

      std::string CCreatedAnswer::body() const
      {
         return std::string();
      }

      EContentType CCreatedAnswer::contentType() const
      {
         return EContentType::kNone;
      }

      std::shared_ptr<std::map<std::string, std::string>> CCreatedAnswer::customHeaders() const
      {
         return m_customHeaders;
      }
   } //namespace rest
} //namespace web 

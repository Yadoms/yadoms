#include "stdafx.h"
#include "CreatedAnswer.h"

namespace web
{
   namespace rest
   {
      CCreatedAnswer::CCreatedAnswer(const std::string& resourceCreatedPath)
         : m_customHeaders(std::make_shared<std::map<std::string, std::string>>())
      {
         (*m_customHeaders)["Location"] = resourceCreatedPath;
      }

      shared::http::ECodes CCreatedAnswer::code() const
      {
         return shared::http::ECodes::kCreated;
      }

      std::string CCreatedAnswer::body() const
      {
         return {};
      }

      IAnswer::EBodyType CCreatedAnswer::bodyType() const
      {
         return EBodyType::kString;
      }

      EContentType CCreatedAnswer::contentType() const
      {
         return EContentType::kNone;
      }

      std::shared_ptr<std::map<std::string, std::string>> CCreatedAnswer::customHeaders() const
      {
         return m_customHeaders;
      }

      std::shared_ptr<service::ISseConnectionHandler> CCreatedAnswer::connectionHandler() const
      {
         return nullptr;
      }
   } //namespace rest
} //namespace web 

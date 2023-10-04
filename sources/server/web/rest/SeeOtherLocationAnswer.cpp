#include "stdafx.h"
#include "SeeOtherLocationAnswer.h"

namespace web
{
   namespace rest
   {
      CSeeOtherLocationAnswer::CSeeOtherLocationAnswer(const std::string& resourceCreatedPath)
         : m_customHeaders(std::make_shared<std::map<std::string, std::string>>())
      {
         (*m_customHeaders)["Location"] = resourceCreatedPath;
      }

      shared::http::ECodes CSeeOtherLocationAnswer::code() const
      {
         return shared::http::ECodes::kSeeOther;
      }

      std::string CSeeOtherLocationAnswer::body() const
      {
         return {};
      }

      IAnswer::EBodyType CSeeOtherLocationAnswer::bodyType() const
      {
         return EBodyType::kString;
      }

      boost::shared_ptr<shared::event::CEventHandler> CSeeOtherLocationAnswer::streamingEventHandler() const
      {
         return nullptr;
      }

      int CSeeOtherLocationAnswer::streamingOnNewEventId() const
      {
         return 0;
      }

      EContentType CSeeOtherLocationAnswer::contentType() const
      {
         return EContentType::kNone;
      }

      std::shared_ptr<std::map<std::string, std::string>> CSeeOtherLocationAnswer::customHeaders() const
      {
         return m_customHeaders;
      }
   } //namespace rest
} //namespace web 

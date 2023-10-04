#include "stdafx.h"
#include "NoContentAnswer.h"

namespace web
{
   namespace rest
   {
      shared::http::ECodes CNoContentAnswer::code() const
      {
         return shared::http::ECodes::kNoContent;
      }

      std::string CNoContentAnswer::body() const
      {
         return {};
      }

      IAnswer::EBodyType CNoContentAnswer::bodyType() const
      {
         return EBodyType::kString;
      }

      boost::shared_ptr<shared::event::CEventHandler> CNoContentAnswer::streamingEventHandler() const
      {
         return nullptr;
      }

      int CNoContentAnswer::streamingOnNewEventId() const
      {
         return 0;
      }

      EContentType CNoContentAnswer::contentType() const
      {
         return EContentType::kNone;
      }

      std::shared_ptr<std::map<std::string, std::string>> CNoContentAnswer::customHeaders() const
      {
         return nullptr;
      }
   } //namespace rest
} //namespace web 

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
         YADOMS_LOG(error) << "Answer " << m_code.toString();
         if (!m_body.empty())
            YADOMS_LOG(error) << " : " + m_body;
      }

      shared::http::ECodes CErrorAnswer::code() const
      {
         return m_code;
      }

      std::string CErrorAnswer::body() const
      {
         return m_body;
      }

      IAnswer::EBodyType CErrorAnswer::bodyType() const
      {
         return EBodyType::kString;
      }

      boost::shared_ptr<shared::event::CEventHandler> CErrorAnswer::streamingEventHandler() const
      {
         return nullptr;
      }

      int CErrorAnswer::streamingOnNewEventId() const
      {
         return 0;
      }

      EContentType CErrorAnswer::contentType() const
      {
         return m_body.empty() ? EContentType::kNone : EContentType::kPlainText;
      }

      std::shared_ptr<std::map<std::string, std::string>> CErrorAnswer::customHeaders() const
      {
         return nullptr;
      }
   } //namespace rest
} //namespace web 

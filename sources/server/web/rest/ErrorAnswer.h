#pragma once
#include "ContentType.h"
#include "IAnswer.h"

namespace web
{
   namespace rest
   {
      class CErrorAnswer final : public IAnswer
      {
      public:
         explicit CErrorAnswer(const shared::http::ECodes& code,
                               std::string message = std::string());
         ~CErrorAnswer() override = default;

         shared::http::ECodes code() const override;
         std::string body() const override;
         EContentType contentType() const override;

      private:
         const shared::http::ECodes m_code;
         const std::string m_body;
      };
   } //namespace rest
} //namespace web 

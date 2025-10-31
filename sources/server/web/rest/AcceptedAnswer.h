#pragma once
#include "IAnswer.h"

namespace web
{
   namespace rest
   {
      class CAcceptedAnswer final : public IAnswer
      {
      public:
         explicit CAcceptedAnswer(const std::string& resourceCreatedPath);
         ~CAcceptedAnswer() override = default;

         shared::http::ECodes code() const override;
         std::string body() const override;
         bool bodyIsFile() const override;
         EContentType contentType() const override;
         std::shared_ptr<std::map<std::string, std::string>> customHeaders() const override;

      private:
         std::shared_ptr<std::map<std::string, std::string>> m_customHeaders;
      };
   } //namespace rest
} //namespace web 

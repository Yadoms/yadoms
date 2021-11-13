#pragma once
#include "IAnswer.h"

namespace web
{
   namespace rest
   {
      class CCreatedAnswer final : public IAnswer
      {
      public:
         explicit CCreatedAnswer(const std::string& resourceCreatedPath);
         ~CCreatedAnswer() override = default;

         shared::http::ECodes code() const override;
         std::string body() const override;
         EContentType contentType() const override;
         std::shared_ptr<std::map<std::string, std::string>> customHeaders() const override;

      private:
         std::shared_ptr<std::map<std::string, std::string>> m_customHeaders = std::make_shared<std::map<std::string, std::string>>();
      };
   } //namespace rest
} //namespace web 

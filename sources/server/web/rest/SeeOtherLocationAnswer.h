#pragma once
#include "IAnswer.h"

namespace web
{
   namespace rest
   {
      class CSeeOtherLocationAnswer final : public IAnswer
      {
      public:
         explicit CSeeOtherLocationAnswer(const std::string& resourceCreatedPath);
         ~CSeeOtherLocationAnswer() override = default;

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
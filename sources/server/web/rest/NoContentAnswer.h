#pragma once
#include "IAnswer.h"

namespace web
{
   namespace rest
   {
      class CNoContentAnswer final : public IAnswer
      {
      public:
         ~CNoContentAnswer() override = default;

         shared::http::ECodes code() const override;
         std::string body() const override;
         bool bodyIsFile() const override;
         EContentType contentType() const override;
         std::shared_ptr<std::map<std::string, std::string>> customHeaders() const override;
      };
   } //namespace rest
} //namespace web 
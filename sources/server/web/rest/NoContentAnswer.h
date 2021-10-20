#pragma once
#include "IAnswer.h"

namespace web
{
   namespace rest
   {
      class CNoContentAnswer final : public IAnswer
      {
      public:
         CNoContentAnswer() = default;
         ~CNoContentAnswer() override = default;

         shared::http::ECodes code() const override;
         std::string body() const override;
         EContentType contentType() const override;
      };
   } //namespace rest
} //namespace web 

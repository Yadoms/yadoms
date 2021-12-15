#pragma once
#include <shared/DataContainer.h>
#include "IAnswer.h"

namespace web
{
   namespace rest
   {
      class CSuccessAnswer final : public IAnswer
      {
      public:
         explicit CSuccessAnswer(const shared::CDataContainer& result);
         CSuccessAnswer(std::string result,
                        const EContentType& contentType = EContentType::kPlainText);
         ~CSuccessAnswer() override = default;

         shared::http::ECodes code() const override;
         std::string body() const override;
         EContentType contentType() const override;
         std::shared_ptr<std::map<std::string, std::string>> customHeaders() const override;

      private:
         const shared::http::ECodes m_code;
         const std::string m_body;
         const EContentType m_contentType;
      };
   } //namespace rest
} //namespace web 

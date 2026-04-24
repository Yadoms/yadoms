#pragma once
#include <shared/DataContainer.h>
#include "IAnswer.h"

namespace web::rest
{
   class CSuccessAnswer final : public IAnswer
   {
   public:
      explicit CSuccessAnswer(const shared::CDataContainer& result);
      explicit CSuccessAnswer(std::string result,
                              const EContentType& contentType = EContentType::kPlainText);
      explicit CSuccessAnswer(const boost::filesystem::path& file,
                              const EContentType& contentType = EContentType::kOctetStream);
      ~CSuccessAnswer() override = default;

      shared::http::ECodes code() const override;
      std::string body() const override;
      bool bodyIsFile() const override;
      EContentType contentType() const override;
      std::shared_ptr<std::map<std::string, std::string>> customHeaders() const override;

   private:
      const bool m_bodyIsFile = false;

      const std::string m_body;
      const EContentType m_contentType;
   };
}

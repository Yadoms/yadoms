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
         explicit CSuccessAnswer(std::string result,
                                 const EContentType& contentType = EContentType::kPlainText);
         explicit CSuccessAnswer(const boost::filesystem::path& file,
                                 const EContentType& contentType = EContentType::kOctetStream);
         explicit CSuccessAnswer(std::shared_ptr<service::ISseConnectionHandler> connectionHandler);
         ~CSuccessAnswer() override = default;

         shared::http::ECodes code() const override;
         std::string body() const override;
         EBodyType bodyType() const override;
         EContentType contentType() const override;
         std::shared_ptr<std::map<std::string, std::string>> customHeaders() const override;
         std::shared_ptr<service::ISseConnectionHandler> connectionHandler() const override;

      private:
         const EBodyType m_bodyType = EBodyType::kString;

         const std::string m_body;
         const EContentType m_contentType;

         const std::shared_ptr<service::ISseConnectionHandler> m_connectionHandler = nullptr;
      };
   } //namespace rest
} //namespace web 

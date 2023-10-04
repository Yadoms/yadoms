#pragma once
#include <shared/DataContainer.h>
#include <shared/event/EventHandler.hpp>
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
         explicit CSuccessAnswer(boost::shared_ptr<shared::event::CEventHandler> streamingEventHandler,
                                 int streamingOnNewEventId);
         ~CSuccessAnswer() override = default;

         shared::http::ECodes code() const override;
         std::string body() const override;
         EBodyType bodyType() const override;
         boost::shared_ptr<shared::event::CEventHandler> streamingEventHandler() const override;
         int streamingOnNewEventId() const override;
         EContentType contentType() const override;
         std::shared_ptr<std::map<std::string, std::string>> customHeaders() const override;

      private:
         const EBodyType m_bodyType = EBodyType::kString;

         const std::string m_body;
         const EContentType m_contentType;
         const boost::shared_ptr<shared::event::CEventHandler>& m_streamingEventHandler = nullptr;
         int m_streamingOnNewEventId = 0;
      };
   } //namespace rest
} //namespace web 

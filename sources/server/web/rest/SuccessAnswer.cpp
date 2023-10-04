#include "stdafx.h"
#include "SuccessAnswer.h"

namespace web
{
   namespace rest
   {
      CSuccessAnswer::CSuccessAnswer(const shared::CDataContainer& result)
         : m_body(result.serialize()),
           m_contentType(EContentType::kJson)
      {
      }

      CSuccessAnswer::CSuccessAnswer(std::string result,
                                     const EContentType& contentType)
         : m_body(std::move(result)),
           m_contentType(contentType)
      {
         if (contentType == EContentType::kOctetStream
            || contentType == EContentType::kImagePng)
            throw std::invalid_argument("Binary types can not be used with string value");
      }

      CSuccessAnswer::CSuccessAnswer(const boost::filesystem::path& file,
                                     const EContentType& contentType)
         : m_bodyType(EBodyType::kFile),
           m_body(file.string()),
           m_contentType(contentType)
      {
      }

      CSuccessAnswer::CSuccessAnswer(boost::shared_ptr<shared::event::CEventHandler> streamingEventHandler,
                                     int streamingOnNewEventId)
         : m_bodyType(EBodyType::kStream),
           m_contentType(EContentType::kTextEventStream),
           m_streamingEventHandler(streamingEventHandler),
           m_streamingOnNewEventId(streamingOnNewEventId)
      {
      }

      shared::http::ECodes CSuccessAnswer::code() const
      {
         return shared::http::ECodes::kOK;
      }

      std::string CSuccessAnswer::body() const
      {
         return m_body;
      }

      IAnswer::EBodyType CSuccessAnswer::bodyType() const
      {
         return m_bodyType;
      }

      boost::shared_ptr<shared::event::CEventHandler> CSuccessAnswer::streamingEventHandler() const
      {
         return m_streamingEventHandler;
      }

      int CSuccessAnswer::streamingOnNewEventId() const
      {
         return m_streamingOnNewEventId;
      }

      EContentType CSuccessAnswer::contentType() const
      {
         return m_contentType;
      }

      std::shared_ptr<std::map<std::string, std::string>> CSuccessAnswer::customHeaders() const
      {
         return nullptr;
      }
   } //namespace rest
} //namespace web 

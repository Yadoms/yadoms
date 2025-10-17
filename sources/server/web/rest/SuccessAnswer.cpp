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
         : m_bodyIsFile(true),
           m_body(file.string()),
           m_contentType(contentType)
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

      bool CSuccessAnswer::bodyIsFile() const
      {
         return m_bodyIsFile;
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

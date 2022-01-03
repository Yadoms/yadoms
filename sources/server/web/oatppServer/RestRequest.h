#pragma once
#include <oatpp/web/server/HttpRequestHandler.hpp>
#include <oatpp/web/mime/multipart/FileStreamProvider.hpp>
#include <oatpp/web/mime/multipart/InMemoryPartReader.hpp>
#include <oatpp/web/mime/multipart/Reader.hpp>
#include <shared/http/Codes.h>
#include "shared/http/HttpRestVerb.h"
#include "web/rest/IRequest.h"

namespace web
{
   namespace oatppServer
   {
      class IOatppFormDataPartHandler
      {
      public:
         virtual ~IOatppFormDataPartHandler() = default;

         virtual std::shared_ptr<oatpp::web::mime::multipart::PartReader> partReader() = 0;
         virtual void setPart(std::shared_ptr<oatpp::web::mime::multipart::Part> part) = 0;
      };

      class CFormDataPartStringHandler final : public IOatppFormDataPartHandler, public rest::IFormDataPartStringHandler //TODO déplacer
      {
      public:
         explicit CFormDataPartStringHandler()
            : m_reader(oatpp::web::mime::multipart::createInMemoryPartReader())
         {
         }

         ~CFormDataPartStringHandler() override = default;

         // IFormDataPartStringHandler Implementation
         std::string string() const override
         {
            if (!m_part)
               return std::string();
            return m_part->getInMemoryData()->c_str();
         }
         // [END] IFormDataPartStringHandler Implementation

         // IFormDataPartStringHandler Implementation
         rest::EContentType contentType() const override
         {
            if (!m_part)
               return rest::EContentType::kNone;
            return rest::ToContentType(m_part->getHeader(oatpp::web::protocol::http::Header::CONTENT_TYPE)->std_str());
         }
         // [END] IFormDataPartStringHandler Implementation

         // IOatppFormDataPartHandler Implementation
         std::shared_ptr<oatpp::web::mime::multipart::PartReader> partReader() override
         {
            return m_reader;
         }

         void setPart(std::shared_ptr<oatpp::web::mime::multipart::Part> part) override
         {
            m_part = part;
         }

         // [END] IOatppFormDataPartHandler Implementation

      private:
         const std::shared_ptr<oatpp::web::mime::multipart::PartReader> m_reader;
         std::shared_ptr<oatpp::web::mime::multipart::Part> m_part;
      };

      class CFormDataPartFileHandler final : public IOatppFormDataPartHandler, public rest::IFormDataPartFileHandler //TODO déplacer
      {
      public:
         explicit CFormDataPartFileHandler(const boost::filesystem::path& into)
            : m_reader(oatpp::web::mime::multipart::createFilePartReader(into.string().c_str()))
         {
         }

         ~CFormDataPartFileHandler() override = default;

         // IFormDataPartFileHandler Implementation
         long long fileSize() const override
         {
            if (!m_part)
               return -1;
            return m_part->getKnownSize();
         }

         std::string fileName() const override
         {
            if (!m_part)
               return std::string();
            return m_part->getFilename()->c_str();
         }
         // [END] IFormDataPartFileHandler Implementation

         // IFormDataPartHandler Implementation
         rest::EContentType contentType() const override
         {
            if (!m_part)
               return rest::EContentType::kNone;
            return rest::ToContentType(m_part->getHeader(oatpp::web::protocol::http::Header::CONTENT_TYPE)->std_str());
         }
         // [END] IFormDataPartHandler Implementation

         // IOatppFormDataPartHandler Implementation
         std::shared_ptr<oatpp::web::mime::multipart::PartReader> partReader() override
         {
            return m_reader;
         }

         void setPart(std::shared_ptr<oatpp::web::mime::multipart::Part> part) override
         {
            m_part = part;
         }

         // [END] IOatppFormDataPartHandler Implementation

      private:
         const std::shared_ptr<oatpp::web::mime::multipart::PartReader> m_reader;
         std::shared_ptr<oatpp::web::mime::multipart::Part> m_part;
      };

      class CRestRequest final : public rest::IRequest
      {
      public:
         explicit CRestRequest(std::shared_ptr<oatpp::web::protocol::http::incoming::Request> request);
         ~CRestRequest() override;

         // rest::IRequest Implementation
         shared::http::ERestVerb method() const override;
         bool pathVariableExists(const std::string& key) const override;
         std::string pathVariable(const std::string& key) const override;
         std::string pathVariable(const std::string& key,
                                  const std::string& defaultValue) const override;
         std::unique_ptr<std::set<std::string>> pathVariableAsList(const std::string& key,
                                                                   char separator) const override;
         bool queryParamExists(const std::string& key) const override;
         std::string queryParam(const std::string& key) const override;
         std::string queryParam(const std::string& key,
                                const std::string& defaultValue) const override;
         std::unique_ptr<std::set<std::string>> queryParamAsList(const std::string& key,
                                                                 char separator) const override;
         boost::shared_ptr<const std::map<std::string, std::string>> queryParams() const override;
         rest::EContentType contentType() const override;
         std::string body() const override;
         float acceptContentType(rest::EContentType contentType) const override;
         boost::shared_ptr<rest::IFormDataPartStringHandler> createFormDataPartStringHandler() override;
         boost::shared_ptr<rest::IFormDataPartFileHandler> createFormDataPartFileHandler(const boost::filesystem::path& path) override;
         void readParts(const std::map<std::string, boost::shared_ptr<rest::IFormDataPartHandler>>& partsHandlers) override;
         // [END] rest::IRequest Implementation

      private:
         static std::map<std::string, std::string> toMap(const oatpp::web::url::mapping::Pattern::MatchMap& in);
         static std::map<std::string, std::string> toMap(const oatpp::web::protocol::http::QueryParams& in);
         static std::unique_ptr<std::set<std::string>> toSet(const std::string& flagsString,
                                                             char separator);

         boost::shared_ptr<std::map<rest::EContentType, float>> parseAcceptContentType() const;
         std::string readBody(std::shared_ptr<oatpp::web::protocol::http::incoming::Request> request) const;

         std::shared_ptr<oatpp::web::protocol::http::incoming::Request> m_request;
         shared::http::ERestVerb m_method;
         mutable boost::shared_ptr<std::map<std::string, std::string>> m_queryParams;
         mutable boost::shared_ptr<std::map<rest::EContentType, float>> m_parsedAcceptContentType;
         mutable std::unique_ptr<const std::string> m_body;
      };
   } //namespace oatppServer
} //namespace web

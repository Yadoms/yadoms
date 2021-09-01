#pragma once
#include <utility>
#include <oatpp/web/server/HttpRequestHandler.hpp>
#include <shared/http/Codes.h>
#include "ContentType.h"
#include "shared/http/HttpRestVerb.h"

//TODO renommer le fichier

namespace web
{
   namespace rest
   {
      class IRestRequest //TODO ménage
      {
      public:
         virtual ~IRestRequest() = default;

         virtual shared::http::ERestVerb method() = 0;

         virtual std::string parameter(const std::string& key) = 0;
         virtual std::string parameter(const std::string& key,
                                       const std::string& defaultValue) = 0;
         virtual std::map<std::string, std::string> parameters() = 0;

         virtual EContentType contentType() = 0;
         virtual std::string body() = 0;

         /// @brief Check if content type is accepted for answer
         /// @param contentType content type to test
         /// @return quality factor (see https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Accept)
         virtual float acceptContentType(EContentType contentType) = 0;
      };

      class COatppRestRequest final : public IRestRequest //TODO ménage
      {
      public:
         explicit COatppRestRequest(std::shared_ptr<oatpp::web::protocol::http::incoming::Request> request)
            : m_request(std::move(request)),
              m_method(shared::http::ToRestVerb(m_request->getStartingLine().method.std_str()))
         {
         }

         ~COatppRestRequest() override = default;

         shared::http::ERestVerb method() override
         {
            return m_method;
         }

         std::string parameter(const std::string& key) override
         {
            const auto value = m_request->getQueryParameter(oatpp::data::share::StringKeyLabel(key.c_str()));
            if (!value)
               throw std::invalid_argument(key + "not found in request");
            return value->std_str();
         }

         std::string parameter(const std::string& key,
                               const std::string& defaultValue) override
         {
            return m_request->getQueryParameter(oatpp::data::share::StringKeyLabel(key.c_str()),
                                                oatpp::String(defaultValue.c_str()))->std_str();
         }

         std::map<std::string, std::string> parameters() override
         {
            return toMap(m_request->getQueryParameters());
         }

         EContentType contentType() override
         {
            return ToContentType(m_request->getHeader(oatpp::web::protocol::http::Header::CONTENT_TYPE)->std_str());
         }

         std::string body() override
         {
            if (m_method == shared::http::ERestVerb::kGet
               || m_method == shared::http::ERestVerb::kHead)
               return std::string();

            return m_request->readBodyToString()->c_str();
         }

         float acceptContentType(EContentType contentType) override
         {
            if (!m_parsedAcceptContentType)
               m_parsedAcceptContentType = parseAcceptContentType();

            const auto it = m_parsedAcceptContentType->find(contentType);
            if (it == m_parsedAcceptContentType->end())
               return 0.0;

            return it->second;
         }

      private:
         static std::map<std::string, std::string> toMap(const oatpp::web::url::mapping::Pattern::MatchMap& in)
         {
            std::map<std::string, std::string> out;
            for (const auto& variable : in.getVariables())
               out.emplace(variable.first.std_str(), variable.second.std_str());
            return out;
         }

         static std::map<std::string, std::string> toMap(const oatpp::web::protocol::http::QueryParams& in)
         {
            std::map<std::string, std::string> out;
            for (const auto& variable : in.getAll())
               out.emplace(variable.first.std_str(), variable.second.std_str());
            return out;
         }

         boost::shared_ptr<std::map<EContentType, float>> parseAcceptContentType() const
         {
            auto map = boost::make_shared<std::map<EContentType, float>>();
            static constexpr float DefaultQuality = 1.0;

            const auto acceptString = m_request->getHeader(oatpp::web::protocol::http::Header::ACCEPT);
            if (!acceptString)
               return map;

            const auto accept = acceptString->std_str();

            // Quality value is not parsed, and all content type are considered to have the same weight (1.0)

            if (accept.find("*/*") != accept.npos)
            {
               map->emplace(EContentType::kOctetStream, DefaultQuality);
               map->emplace(EContentType::kPlainText, DefaultQuality);
               map->emplace(EContentType::kJson, DefaultQuality);
               return map;
            }

            if (accept.find(ContentTypeOctetStreamString) != accept.npos)
               map->emplace(EContentType::kOctetStream, DefaultQuality);
            if (accept.find(ContentTypePlainTextString) != accept.npos)
               map->emplace(EContentType::kPlainText, DefaultQuality);
            if (accept.find(ContentTypeJsonString) != accept.npos)
               map->emplace(EContentType::kJson, DefaultQuality);

            return map;
         }

         std::shared_ptr<oatpp::web::protocol::http::incoming::Request> m_request;
         shared::http::ERestVerb m_method;
         boost::shared_ptr<std::map<EContentType, float>> m_parsedAcceptContentType;
      };
   } //namespace rest
} //namespace web 

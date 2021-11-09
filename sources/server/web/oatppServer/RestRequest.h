#pragma once
#include <oatpp/web/server/HttpRequestHandler.hpp>
#include <shared/http/Codes.h>
#include "shared/http/HttpRestVerb.h"
#include "web/rest/IRequest.h"

namespace web
{
   namespace oatppServer
   {
      class CRestRequest final : public rest::IRequest
      {
      public:
         explicit CRestRequest(std::shared_ptr<oatpp::web::protocol::http::incoming::Request> request);
         ~CRestRequest() override = default;

         shared::http::ERestVerb method() override;
         std::string parameter(const std::string& key) override;
         std::string parameter(const std::string& key,
                               const std::string& defaultValue) override;
         std::unique_ptr<std::set<std::string>> parameterAsFlagList(const std::string& key) override;
         std::map<std::string, std::string> parameters() override;
         rest::EContentType contentType() override;
         std::string body() override;
         float acceptContentType(rest::EContentType contentType) override;

      private:
         static std::map<std::string, std::string> toMap(const oatpp::web::url::mapping::Pattern::MatchMap& in);
         static std::map<std::string, std::string> toMap(const oatpp::web::protocol::http::QueryParams& in);
         boost::shared_ptr<std::map<rest::EContentType, float>> parseAcceptContentType() const;
         std::string readBody(std::shared_ptr<oatpp::web::protocol::http::incoming::Request> request) const;

         std::shared_ptr<oatpp::web::protocol::http::incoming::Request> m_request;
         shared::http::ERestVerb m_method;
         boost::shared_ptr<std::map<rest::EContentType, float>> m_parsedAcceptContentType;
         const std::string m_body;
      };
   } //namespace oatppServer
} //namespace web

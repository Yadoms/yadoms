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
         const std::string m_body;
      };
   } //namespace oatppServer
} //namespace web

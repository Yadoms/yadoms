#include "stdafx.h"
#include "RestRequest.h"

namespace web
{
   namespace oatppServer
   {
      CRestRequest::CRestRequest(std::shared_ptr<oatpp::web::protocol::http::incoming::Request> request)
         : m_request(std::move(request)),
           m_method(shared::http::ToRestVerb(m_request->getStartingLine().method.std_str()))
      {
      }

      shared::http::ERestVerb CRestRequest::method()
      {
         return m_method;
      }

      std::string CRestRequest::parameter(const std::string& key)
      {
         const auto value = m_request->getQueryParameter(oatpp::data::share::StringKeyLabel(key.c_str()));
         if (!value)
            throw std::invalid_argument(key + "not found in request");
         return value->std_str();
      }

      std::string CRestRequest::parameter(const std::string& key,
                                          const std::string& defaultValue)
      {
         return m_request->getQueryParameter(oatpp::data::share::StringKeyLabel(key.c_str()),
                                             oatpp::String(defaultValue.c_str()))->std_str();
      }

      std::map<std::string, std::string> CRestRequest::parameters()
      {
         return toMap(m_request->getQueryParameters());
      }

      rest::EContentType CRestRequest::contentType()
      {
         return rest::ToContentType(m_request->getHeader(oatpp::web::protocol::http::Header::CONTENT_TYPE)->std_str());
      }

      std::string CRestRequest::body()
      {
         if (m_method == shared::http::ERestVerb::kGet
            || m_method == shared::http::ERestVerb::kHead
            || m_method == shared::http::ERestVerb::kDelete)
            return std::string();

         return m_request->readBodyToString()->c_str();
      }

      float CRestRequest::acceptContentType(rest::EContentType contentType)
      {
         if (!m_parsedAcceptContentType)
            m_parsedAcceptContentType = parseAcceptContentType();

         const auto it = m_parsedAcceptContentType->find(contentType);
         if (it == m_parsedAcceptContentType->end())
            return 0.0;

         return it->second;
      }

      std::map<std::string, std::string> CRestRequest::toMap(const oatpp::web::url::mapping::Pattern::MatchMap& in)
      {
         std::map<std::string, std::string> out;
         for (const auto& variable : in.getVariables())
            out.emplace(variable.first.std_str(), variable.second.std_str());
         return out;
      }

      std::map<std::string, std::string> CRestRequest::toMap(const oatpp::web::protocol::http::QueryParams& in)
      {
         std::map<std::string, std::string> out;
         for (const auto& variable : in.getAll())
            out.emplace(variable.first.std_str(), variable.second.std_str());
         return out;
      }

      boost::shared_ptr<std::map<rest::EContentType, float>> CRestRequest::parseAcceptContentType() const
      {
         auto map = boost::make_shared<std::map<rest::EContentType, float>>();
         static constexpr float DefaultQuality = 1.0;

         const auto acceptString = m_request->getHeader(oatpp::web::protocol::http::Header::ACCEPT);
         if (!acceptString)
            return map;

         const auto accept = acceptString->std_str();

         // Quality value is not parsed, and all content type are considered to have the same weight (1.0)

         if (accept.find("*/*") != std::string::npos)
         {
            map->emplace(rest::EContentType::kOctetStream, DefaultQuality);
            map->emplace(rest::EContentType::kPlainText, DefaultQuality);
            map->emplace(rest::EContentType::kJson, DefaultQuality);
            return map;
         }

         if (accept.find(rest::ContentTypeOctetStreamString) != std::string::npos)
            map->emplace(rest::EContentType::kOctetStream, DefaultQuality);
         if (accept.find(rest::ContentTypePlainTextString) != std::string::npos)
            map->emplace(rest::EContentType::kPlainText, DefaultQuality);
         if (accept.find(rest::ContentTypeJsonString) != std::string::npos)
            map->emplace(rest::EContentType::kJson, DefaultQuality);

         return map;
      }
   } //namespace oatppServer
} //namespace web 

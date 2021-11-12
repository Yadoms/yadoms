#include "stdafx.h"
#include "RestRequest.h"

namespace web
{
   namespace oatppServer
   {
      CRestRequest::CRestRequest(std::shared_ptr<oatpp::web::protocol::http::incoming::Request> request)
         : m_request(std::move(request)),
           m_method(shared::http::ToRestVerb(m_request->getStartingLine().method.std_str())),
           m_body(readBody(m_request)) // Need to consume body for each request (or next request will be malformed and oatpp will answer 404)
      {
      }

      shared::http::ERestVerb CRestRequest::method()
      {
         return m_method;
      }

      std::string CRestRequest::pathVariable(const std::string& key)
      {
         const auto value = m_request->getPathVariable(key.c_str());
         if (!value)
            throw std::invalid_argument(key + "not found in request");
         return value->std_str();
      }

      std::string CRestRequest::pathVariable(const std::string& key,
                                             const std::string& defaultValue)
      {
         const auto value = m_request->getPathVariable(key.c_str());
         if (!value)
            return defaultValue;
         return value->std_str();
      }

      bool CRestRequest::queryParamExists(const std::string& key)
      {
         const auto params = queryParams();
         return params->find(key) != params->end();
      }

      std::string CRestRequest::queryParam(const std::string& key)
      {
         const auto params = queryParams();
         const auto param = params->find(key);
         if (param == params->end())
            throw std::invalid_argument(key + "not found in request");

         return param->second;
      }

      std::string CRestRequest::queryParam(const std::string& key,
                                           const std::string& defaultValue)
      {
         const auto params = queryParams();
         const auto param = params->find(key);
         if (param == params->end())
            return defaultValue;

         return param->second;
      }

      std::unique_ptr<std::set<std::string>> CRestRequest::queryParamAsList(const std::string& key,
                                                                            char separator)
      {
         const auto flagsString = queryParam(key, std::string());
         if (flagsString.empty())
            return std::make_unique<std::set<std::string>>();

         auto flags = std::make_unique<std::set<std::string>>();
         for (const auto& t : boost::tokenizer<boost::char_separator<char>>(flagsString,
                                                                            boost::char_separator<char>(&separator)))
            flags->insert(t);

         return flags;
      }

      boost::shared_ptr<const std::map<std::string, std::string>> CRestRequest::queryParams()
      {
         if (!m_queryParams)
            m_queryParams = boost::make_shared<std::map<std::string, std::string>>(toMap(m_request->getQueryParameters()));
         return m_queryParams;
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

         return m_body;
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

      std::string CRestRequest::readBody(std::shared_ptr<oatpp::web::protocol::http::incoming::Request> request) const
      {
         // Need to consume body for each body-containing request (if not next request will be malformed and oatpp will answer 404)

         if (m_method == shared::http::ERestVerb::kGet
            || m_method == shared::http::ERestVerb::kHead
            || m_method == shared::http::ERestVerb::kDelete)
            return std::string();

         return request->readBodyToString()->c_str();
      }
   } //namespace oatppServer
} //namespace web 

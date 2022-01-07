#include "stdafx.h"

#include "RestRequest.h"
#include <regex>
#include <oatpp/web/mime/multipart/PartList.hpp>
#include "FormDataPartFileHandler.h"
#include "FormDataPartStringHandler.h"
#include "shared/http/HttpHelpers.h"

namespace web
{
   namespace oatppServer
   {
      CRestRequest::CRestRequest(std::shared_ptr<oatpp::web::protocol::http::incoming::Request> request)
         : m_request(std::move(request)),
           m_method(shared::http::ToRestVerb(m_request->getStartingLine().method.std_str()))
      {
      }

      CRestRequest::~CRestRequest()
      {
         // Need to consume body for each request (or next request will be malformed and oatpp will answer 404)
         if (!m_body)
            // ReSharper disable once CppExpressionWithoutSideEffects
            readBody(m_request);
      }

      shared::http::ERestVerb CRestRequest::method() const
      {
         return m_method;
      }

      bool CRestRequest::pathVariableExists(const std::string& key) const
      {
         return !!m_request->getPathVariable(key.c_str());
      }

      std::string CRestRequest::pathVariable(const std::string& key) const
      {
         const auto value = m_request->getPathVariable(key.c_str());
         if (!value)
            throw std::invalid_argument(key + "not found in request");
         return shared::http::CHttpHelpers::urlDecode(value->std_str());
      }

      std::string CRestRequest::pathVariable(const std::string& key,
                                             const std::string& defaultValue) const
      {
         const auto value = m_request->getPathVariable(key.c_str());
         if (!value)
            return defaultValue;
         return shared::http::CHttpHelpers::urlDecode(value->std_str());
      }

      std::unique_ptr<std::set<std::string>> CRestRequest::pathVariableAsList(const std::string& key,
                                                                              char separator) const
      {
         return toSet(pathVariable(key, std::string()),
                      separator);
      }

      bool CRestRequest::queryParamExists(const std::string& key) const
      {
         const auto params = queryParams();
         return params->find(key) != params->end();
      }

      std::string CRestRequest::queryParam(const std::string& key) const
      {
         const auto params = queryParams();
         const auto param = params->find(key);
         if (param == params->end())
            throw std::invalid_argument(key + "not found in request");

         return param->second;
      }

      std::string CRestRequest::queryParam(const std::string& key,
                                           const std::string& defaultValue) const
      {
         const auto params = queryParams();
         const auto param = params->find(key);
         if (param == params->end())
            return defaultValue;

         return param->second;
      }

      std::unique_ptr<std::set<std::string>> CRestRequest::queryParamAsList(const std::string& key,
                                                                            char separator) const
      {
         return toSet(queryParam(key, std::string()),
                      separator);
      }

      boost::shared_ptr<const std::map<std::string, std::string>> CRestRequest::queryParams() const
      {
         if (!m_queryParams)
            m_queryParams = boost::make_shared<std::map<std::string, std::string>>(toMap(m_request->getQueryParameters()));
         return m_queryParams;
      }

      rest::EContentType CRestRequest::contentType() const
      {
         return rest::ToContentType(m_request->getHeader(oatpp::web::protocol::http::Header::CONTENT_TYPE)->std_str());
      }

      std::string CRestRequest::body() const
      {
         if (m_method == shared::http::ERestVerb::kGet
            || m_method == shared::http::ERestVerb::kHead
            || m_method == shared::http::ERestVerb::kDelete)
            return std::string();

         if (!m_body)
            m_body = std::make_unique<const std::string>(readBody(m_request));
         return *m_body;
      }

      float CRestRequest::acceptContentType(rest::EContentType contentType) const
      {
         if (!m_parsedAcceptContentType)
            m_parsedAcceptContentType = parseAcceptContentType();

         const auto it = m_parsedAcceptContentType->find(contentType);
         if (it == m_parsedAcceptContentType->end())
            return 0.0;

         return it->second;
      }

      boost::shared_ptr<rest::IFormDataPartStringHandler> CRestRequest::createFormDataPartStringHandler()
      {
         return boost::make_shared<CFormDataPartStringHandler>();
      }

      boost::shared_ptr<rest::IFormDataPartFileHandler> CRestRequest::createFormDataPartFileHandler(const boost::filesystem::path& path)
      {
         return boost::make_shared<CFormDataPartFileHandler>(path);
      }

      void CRestRequest::readParts(const std::map<std::string, boost::shared_ptr<rest::IFormDataPartHandler>>& partsHandlers)
      {
         const auto multipart = std::make_shared<oatpp::web::mime::multipart::PartList>(m_request->getHeaders());
         oatpp::web::mime::multipart::Reader multipartReader(multipart.get());

         // Set part handlers
         for (const auto& partHandler : partsHandlers)
         {
            const auto oatppFormDataPartHandler = boost::dynamic_pointer_cast<IOatppFormDataPartHandler>(partHandler.second);
            multipartReader.setPartReader(partHandler.first.c_str(),
                                          oatppFormDataPartHandler->partReader());
         }

         // Mark body as read (it will be read by transferBody, even if it fails with exception)
         m_body = std::make_unique<const std::string>();

         // Read body and populate part handlers
         m_request->transferBody(&multipartReader);

         // Extract parts
         for (const auto& partHandler : partsHandlers)
         {
            const auto oatppFormDataPartHandler = boost::dynamic_pointer_cast<IOatppFormDataPartHandler>(partHandler.second);
            oatppFormDataPartHandler->setPart(multipart->getNamedPart(partHandler.first.c_str()));
         }
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
            out.emplace(variable.first.std_str(), shared::http::CHttpHelpers::urlDecode(variable.second.std_str()));
         return out;
      }

      std::unique_ptr<std::set<std::string>> CRestRequest::toSet(const std::string& flagsString,
                                                                 char separator)
      {
         if (flagsString.empty())
            return std::make_unique<std::set<std::string>>();

         auto flags = std::make_unique<std::set<std::string>>();
         for (const auto& t : boost::tokenizer<boost::char_separator<char>>(flagsString,
                                                                            boost::char_separator<char>(&separator)))
            flags->insert(t);

         return flags;
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

#include "stdafx.h"
#include "HttpMethod.h"
#include <regex>
#include <utility>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Infos.hpp>
#include <shared/Log.h>
#include <shared/exception/HttpException.hpp>
#include "Codes.h"
#include "Proxy.h"

namespace shared
{
   namespace http
   {
      class CCurlResources final
      {
      public:
         CCurlResources()
         {
            cURLpp::initialize(CURL_GLOBAL_ALL);
         }

         ~CCurlResources()
         {
            cURLpp::terminate();
         }
      };

      // ReSharper disable once CppDeclaratorNeverUsed
      static CCurlResources CurlResources;


      CHttpMethod::CHttpMethod(ERequest requesType,
                               std::string url)
         : m_requestType(requesType),
           m_url(std::move(url)),
           m_timeoutSeconds(HttpRequestDefaultTimeoutSeconds)
      {
      }

      CHttpMethod& CHttpMethod::withHeaderParameters(const std::map<std::string, std::string>& headerParameters)
      {
         m_headerParameters = headerParameters;
         return *this;
      }

      CHttpMethod& CHttpMethod::withParameters(const std::map<std::string, std::string>& parameters)
      {
         m_parameters = parameters;
         return *this;
      }

      CHttpMethod& CHttpMethod::withTimeout(int timeoutSeconds)
      {
         m_timeoutSeconds = timeoutSeconds;
         return *this;
      }

      CHttpMethod& CHttpMethod::withBody(const std::string& body)
      {
         if (m_requestType != ERequest::kPost)
            throw std::runtime_error("CHttpMethod : body is only supported for POST requests");
         m_body = body;
         return *this;
      }

      void CHttpMethod::send(const std::function<void(const std::map<std::string, std::string>& receivedHeaders,
                                                      const std::string& data)>& responseHandlerFct)
      {
         switch (m_requestType)
         {
         case ERequest::kGet:
            m_request.setOpt(new curlpp::options::HttpGet(true));
            break;
         case ERequest::kHead:
            m_request.setOpt(new curlpp::options::HttpGet(true));
            m_request.setOpt(new curlpp::options::NoBody(true));
            break;
         case ERequest::kPost:
            m_request.setOpt(new curlpp::options::PostFields(m_body));
            break;
         case ERequest::kPut:
            m_request.setOpt(new curlpp::options::CustomRequest("PUT"));
            m_request.setOpt(new curlpp::options::PostFields(m_body));
            break;
         }

         m_request.setOpt(new curlpp::options::Timeout(m_timeoutSeconds));

         // Proxy
         if (CProxy::available())
            setProxy(m_url,
                     CProxy::getHost(),
                     CProxy::getPort(),
                     CProxy::getUsername(),
                     CProxy::getPassword(),
                     CProxy::getBypassRegex());

         // Follow redirections
         m_request.setOpt(new curlpp::options::FollowLocation(true));
         m_request.setOpt(new curlpp::options::MaxRedirs(3));

         // URL + parameters
         m_request.setOpt(
            new curlpp::options::Url(m_url + stringifyParameters(m_parameters)));

         // HTTPS support : skip peer and host verification
         static const std::string HttpsHeader("https://");
         if (std::search(m_url.begin(), m_url.end(),
                         HttpsHeader.begin(), HttpsHeader.end(),
                         [](const char ch1, const char ch2)
                         {
                            return std::tolower(ch1) == std::tolower(ch2);
                         })
            != m_url.end())
         {
            m_request.setOpt(new curlpp::options::SslVerifyPeer(false));
            m_request.setOpt(new curlpp::options::SslVerifyHost(false));
         }

         // Headers
         setHeaders(m_headerParameters);

         // Response headers
         std::string headersBuffer;
         m_request.setOpt(curlpp::options::HeaderFunction(
            [&headersBuffer](char* ptr, size_t size, size_t nbItems)
            {
               const auto incomingSize = size * nbItems;
               headersBuffer.append(ptr, incomingSize);
               return incomingSize;
            }));

         // Response data
         std::string dataBuffer;
         if (m_requestType != ERequest::kHead)
         {
            m_request.setOpt(curlpp::options::WriteFunction(
               [&dataBuffer](char* ptr, size_t size, size_t nbItems)
               {
                  const auto incomingSize = size * nbItems;
                  dataBuffer.append(ptr, incomingSize);
                  return incomingSize;
               }));
         }

         try
         {
            m_request.perform();
         }
         catch (const curlpp::LibcurlRuntimeError& error)
         {
            const auto message = (boost::format("Fail to send HTTP request : %1%, code %2%") % error.what() % error.
               whatCode()).str();
            YADOMS_LOG(warning) << message;
            if (ECodes::isDefined(curlpp::infos::ResponseCode::get(m_request)))
               throw exception::CHttpException(message, ECodes(curlpp::infos::ResponseCode::get(m_request)));
            throw std::runtime_error(message);
         }

         checkResult();

         responseHandlerFct(formatResponseHeaders(headersBuffer),
                            dataBuffer);
      }

      void CHttpMethod::setProxy(const std::string& url,
                                 const std::string& proxyHost,
                                 int proxyPort,
                                 const std::string& proxyUsername,
                                 const std::string& proxyPassword,
                                 const std::string& proxyBypassRegex)
      {
         if (!proxyBypassRegex.empty() && std::regex_search(url, std::regex(proxyBypassRegex)))
            return;

         m_request.setOpt(new curlpp::options::Proxy(proxyHost));
         if (proxyPort != CProxy::kUseProxyDefaultPort)
            m_request.setOpt(new curlpp::options::ProxyPort(proxyPort));
         if (!proxyUsername.empty() && !proxyPassword.empty())
            m_request.setOpt(new curlpp::options::ProxyUserPwd(proxyUsername + ":" + proxyPassword));
      }

      std::string CHttpMethod::stringifyParameters(const std::map<std::string, std::string>& parameters)
      {
         if (parameters.empty())
            return std::string();

         std::string urlSuffix;
         for (const auto& parameter : parameters)
         {
            urlSuffix += urlSuffix.empty() ? "?" : "&";
            urlSuffix += curlpp::escape(parameter.first) + "=" + curlpp::escape(parameter.second);
         }
         return urlSuffix;
      }

      void CHttpMethod::setHeaders(const std::map<std::basic_string<char>, std::basic_string<char>>& headerParameters)
      {
         if (headerParameters.empty())
            return;

         std::list<std::string> headers;
         for (const auto& headerParametersIterator : headerParameters)
            headers.push_back(headerParametersIterator.first + ": " + headerParametersIterator.second);

         m_request.setOpt(new curlpp::options::HttpHeader(headers));
      }

      void CHttpMethod::checkResult() const
      {
         if (curlpp::infos::ResponseCode::get(m_request) != ECodes::kOKValue &&
            curlpp::infos::ResponseCode::get(m_request) != ECodes::kCreatedValue)
         {
            const auto message = (boost::format("Invalid HTTP result : %1%") %
               curlpp::infos::ResponseCode::get(m_request)).str();
            YADOMS_LOG(warning) << message;
            throw exception::CHttpException(message, ECodes(curlpp::infos::ResponseCode::get(m_request)));
         }
      }

      std::map<std::string, std::string> CHttpMethod::formatResponseHeaders(const std::string& headersBuffer) const
      {
         std::vector<std::string> headerKeyValues;
         split(headerKeyValues, headersBuffer, boost::is_any_of("\n"), boost::algorithm::token_compress_on);

         std::map<std::string, std::string> responseHeaders;
         for (const auto& headerKeyValue : headerKeyValues)
         {
            const auto separatorIterator = headerKeyValue.find(':');
            if (separatorIterator == std::string::npos)
               continue;

            // Http headers are not case-sensitive
            // Make all lower to facilitate search and comparison

            auto key = headerKeyValue.substr(0, separatorIterator);
            boost::to_lower(key);

            auto value = headerKeyValue.substr(separatorIterator + 1, std::string::npos);
            boost::to_lower(value);

            responseHeaders[key] = value;
         }

         return responseHeaders;
      }
   }
} // namespace shared::http

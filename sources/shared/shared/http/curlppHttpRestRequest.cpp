#include "stdafx.h"
#include "curlppHttpRestRequest.h"
#include <regex>
#include <utility>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Infos.hpp>
#include <shared/Log.h>
#include <shared/exception/HttpException.hpp>
#include "Codes.h"
#include "curlppHelpers.h"

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


      CCurlppHttpRestRequest::CCurlppHttpRestRequest(ERestMethod requestType,
                                                     std::string url)
         : m_requestType(requestType),
           m_url(std::move(url)),
           m_timeoutSeconds(HttpRequestDefaultTimeoutSeconds)
      {
      }

      IHttpRestRequest& CCurlppHttpRestRequest::withHeaderParameters(const std::map<std::string, std::string>& headerParameters)
      {
         m_headerParameters = headerParameters;
         return *this;
      }

      IHttpRestRequest& CCurlppHttpRestRequest::withParameters(const std::map<std::string, std::string>& parameters)
      {
         m_parameters = parameters;
         return *this;
      }

      IHttpRestRequest& CCurlppHttpRestRequest::withTimeout(int timeoutSeconds)
      {
         m_timeoutSeconds = timeoutSeconds;
         return *this;
      }

      IHttpRestRequest& CCurlppHttpRestRequest::withBody(const std::string& body)
      {
         if (m_requestType != ERestMethod::kPost && m_requestType != ERestMethod::kPut)
            throw std::runtime_error("CCurlppHttpRestRequest : body is only supported for POST and PUT requests");
         m_body = body;
         return *this;
      }

      IHttpRestRequest& CCurlppHttpRestRequest::withBasicAuthentication(const std::string& user,
                                                                        const std::string& password)
      {
         m_request.setOpt(curlpp::options::HttpAuth(CURLAUTH_BASIC));
         m_request.setOpt(curlpp::options::UserPwd(user + ":" + password));
         return *this;
      }

      void CCurlppHttpRestRequest::send(const std::function<void(const std::map<std::string, std::string>& receivedHeaders,
                                                                 const std::string& data)>& responseHandlerFct)
      {
         switch (m_requestType)
         {
         case ERestMethod::kGet:
            m_request.setOpt(new curlpp::options::HttpGet(true));
            break;
         case ERestMethod::kHead:
            m_request.setOpt(new curlpp::options::HttpGet(true));
            m_request.setOpt(new curlpp::options::NoBody(true));
            break;
         case ERestMethod::kPost:
            m_request.setOpt(new curlpp::options::PostFields(m_body));
            break;
         case ERestMethod::kPut:
            m_request.setOpt(new curlpp::options::CustomRequest("PUT"));
            m_request.setOpt(new curlpp::options::PostFields(m_body));
            break;
         }

         m_request.setOpt(new curlpp::options::Timeout(m_timeoutSeconds));

         // Proxy         
         CCurlppHelpers::setProxy(m_request,
                                  m_url);

         // Follow redirections
         m_request.setOpt(new curlpp::options::FollowLocation(true));
         m_request.setOpt(new curlpp::options::MaxRedirs(3));

         // URL + parameters
         m_request.setOpt(
            new curlpp::options::Url(m_url + CCurlppHelpers::stringifyParameters(m_parameters)));

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
         CCurlppHelpers::setHeaders(m_request,
                                    m_headerParameters);

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
         if (m_requestType != ERestMethod::kHead)
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

         CCurlppHelpers::checkResult(m_request);

         responseHandlerFct(formatResponseHeaders(headersBuffer),
                            dataBuffer);
      }

      void CCurlppHttpRestRequest::send(const std::function<void(boost::shared_ptr<CDataContainer> data)>& responseHandlerFct)
      {
         send([&responseHandlerFct, this](const std::map<std::string, std::string>& receivedHeaders,
                                          const std::string& data)
         {
            responseHandlerFct(processJsonResponse(receivedHeaders,
                                                   data));
         });
      }

      std::string CCurlppHttpRestRequest::send()
      {
         std::string out;
         send([&out](const std::map<std::string, std::string>& receivedHeaders,
                     const std::string& data)
         {
            out = data;
         });
         return out;
      }

      std::map<std::string, std::string> CCurlppHttpRestRequest::formatResponseHeaders(const std::string& headersBuffer) const
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

      boost::shared_ptr<CDataContainer> CCurlppHttpRestRequest::processJsonResponse(const std::map<std::string, std::string>& receivedHeaders,
                                                                                    const std::string& data) const
      {
         try
         {
            const auto& contentType = receivedHeaders.at("content-type");
            if (contentType.find("application/json") == std::string::npos &&
               contentType.find("text/json") == std::string::npos)
               throw std::runtime_error(
                  std::string("Content type was not defined as JSON or text : ") + contentType);
         }
         catch (const std::exception& exception)
         {
            throw std::runtime_error(std::string("Fail to process HTTP JSON answer : ") + exception.what());
         }

         // Content-Length is not always fulfilled so we don't use hasContentLength and getContentLength
         return CDataContainer::make(data);
      }
   }
} // namespace shared::http

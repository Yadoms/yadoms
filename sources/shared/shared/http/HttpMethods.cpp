#include "stdafx.h"
#include "HttpMethods.h"
#include <shared/exception/HttpException.hpp>
#include <shared/Log.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>
#include <regex>
#include "curlppHelpers.h"
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


      void CHttpMethods::sendGetRequest(const std::string& url,
                                        const boost::function<void(
                                           const std::map<std::string, std::string>& receivedHeaders,
                                           const std::string& data)>& responseHandlerFct,
                                        const std::map<std::string, std::string>& headerParameters,
                                        const std::map<std::string, std::string>& parameters,
                                        int timeoutSeconds)
      {
         curlpp::Easy request;

         request.setOpt(new curlpp::options::HttpGet(true));

         request.setOpt(new curlpp::options::Timeout(timeoutSeconds));

         // Proxy
         if (CProxy::available())
            CCurlppHelpers::setProxy(request,
                                     url,
                                     CProxy::getHost(),
                                     CProxy::getPort(),
                                     CProxy::getUsername(),
                                     CProxy::getPassword(),
                                     CProxy::getBypassRegex());

         // Follow redirections
         request.setOpt(new curlpp::options::FollowLocation(true));
         request.setOpt(new curlpp::options::MaxRedirs(3));

         // URL + parameters
         request.setOpt(
            new curlpp::options::Url(url + CCurlppHelpers::stringifyParameters(parameters)));

         // HTTPS support : skip peer and host verification
         static const std::string HttpsHeader("https://");
         if (std::search(url.begin(), url.end(),
                         HttpsHeader.begin(), HttpsHeader.end(),
                         [](const char ch1, const char ch2)
                         {
                            return std::tolower(ch1) == std::tolower(ch2);
                         })
            != url.end())
         {
            request.setOpt(new curlpp::options::SslVerifyPeer(false));
            request.setOpt(new curlpp::options::SslVerifyHost(false));
         }

         // Headers
         CCurlppHelpers::setHeaders(request, headerParameters);

         // Response headers
         std::string headersBuffer;
         request.setOpt(curlpp::options::HeaderFunction(
            [&headersBuffer](char* ptr, size_t size, size_t nbItems)
            {
               const auto incomingSize = size * nbItems;
               headersBuffer.append(ptr, incomingSize);
               return incomingSize;
            }));

         // Response data
         std::string dataBuffer;
         request.setOpt(curlpp::options::WriteFunction(
            [&dataBuffer](char* ptr, size_t size, size_t nbItems)
            {
               const auto incomingSize = size * nbItems;
               dataBuffer.append(ptr, incomingSize);
               return incomingSize;
            }));

         try
         {
            request.perform();
         }
         catch (const curlpp::LibcurlRuntimeError& error)
         {
            const auto message = (boost::format("Fail to send GET HTTP request : %1%, code %2%") % error.what() % error.
               whatCode()).str();
            YADOMS_LOG(warning) << message;
            throw exception::CHttpException(message);
         }

         CCurlppHelpers::checkResult(request);

         responseHandlerFct(CCurlppHelpers::formatResponseHeaders(headersBuffer),
                            dataBuffer);
      }

      std::string CHttpMethods::sendGetRequest(const std::string& url,
                                               const std::map<std::string, std::string>& headerParameters,
                                               const std::map<std::string, std::string>& parameters,
                                               int timeoutSeconds)
      {
         std::string out;
         sendGetRequest(url,
                        [&out](const std::map<std::string, std::string>& receivedHeaders,
                               const std::string& data)
                        {
                           out = data;
                        },
                        headerParameters,
                        parameters,
                        timeoutSeconds);

         return out;
      }

      boost::shared_ptr<CDataContainer> CHttpMethods::sendJsonGetRequest(
         const std::string& url,
         const std::map<std::string, std::string>& headerParameters,
         const std::map<std::string, std::string>& parameters,
         int timeoutSeconds)
      {
         boost::shared_ptr<CDataContainer> out;
         sendGetRequest(url,
                        [&out](const std::map<std::string, std::string>& receivedHeaders,
                               const std::string& data)
                        {
                           out = processJsonResponse(receivedHeaders,
                                                     data);
                        },
                        headerParameters,
                        parameters,
                        timeoutSeconds);

         return out;
      }

      void CHttpMethods::sendPostRequest(const std::string& url,
                                         const std::string& body,
                                         const boost::function<void(
                                            const std::map<std::string, std::string>& receivedHeaders,
                                            const std::string& data)>& responseHandlerFct,
                                         const std::map<std::string, std::string>& headerParameters,
                                         const std::map<std::string, std::string>& parameters,
                                         int timeoutSeconds)
      {
         curlpp::Easy request;

         request.setOpt(new curlpp::options::PostFields(body));

         request.setOpt(new curlpp::options::Timeout(timeoutSeconds));

         // Proxy
         if (CProxy::available())
            CCurlppHelpers::setProxy(request,
                                     url,
                                     CProxy::getHost(),
                                     CProxy::getPort(),
                                     CProxy::getUsername(),
                                     CProxy::getPassword(),
                                     CProxy::getBypassRegex());

         // Follow redirections
         request.setOpt(new curlpp::options::FollowLocation(true));
         request.setOpt(new curlpp::options::MaxRedirs(3));

         // URL + parameters
         request.setOpt(
            new curlpp::options::Url(url + CCurlppHelpers::stringifyParameters(parameters)));

         // Headers
         CCurlppHelpers::setHeaders(request, headerParameters);

         // Response headers
         std::string headersBuffer;
         request.setOpt(curlpp::options::HeaderFunction(
            [&headersBuffer](char* ptr, size_t size, size_t nbItems)
            {
               const auto incomingSize = size * nbItems;
               headersBuffer.append(ptr, incomingSize);
               return incomingSize;
            }));

         // Response data
         std::string dataBuffer;
         request.setOpt(curlpp::options::WriteFunction(
            [&dataBuffer](char* ptr, size_t size, size_t nbItems)
            {
               const auto incomingSize = size * nbItems;
               dataBuffer.append(ptr, incomingSize);
               return incomingSize;
            }));

         try
         {
            request.perform();
         }
         catch (const curlpp::LibcurlRuntimeError& error)
         {
            const auto message = (boost::format("Fail to send GET HTTP request : %1%, code %2%") % error.what() % error.
               whatCode()).str();
            YADOMS_LOG(warning) << message;
            throw exception::CHttpException(message);
         }

         CCurlppHelpers::checkResult(request);

         responseHandlerFct(CCurlppHelpers::formatResponseHeaders(headersBuffer),
                            dataBuffer);
      }

      std::string CHttpMethods::sendPostRequest(
         const std::string& url,
         const std::string& body,
         const std::map<std::string, std::string>& headerParameters,
         const std::map<std::string, std::string>& parameters,
         int timeoutSeconds)
      {
         std::string out;
         sendPostRequest(url,
                         body,
                         [&out](const std::map<std::string, std::string>& receivedHeaders,
                                const std::string& data)
                         {
                            out = data;
                         },
                         headerParameters,
                         parameters,
                         timeoutSeconds);

         return out;
      }

      boost::shared_ptr<CDataContainer> CHttpMethods::sendJsonPostRequest(
         const std::string& url,
         const std::string& body,
         const std::map<std::string, std::string>& headerParameters,
         const std::map<std::string, std::string>& parameters,
         int timeoutSeconds)
      {
         boost::shared_ptr<CDataContainer> out;
         sendPostRequest(url,
                         body,
                         [&out](const std::map<std::string, std::string>& receivedHeaders,
                                const std::string& data)
                         {
                            out = processJsonResponse(receivedHeaders,
                                                      data);
                         },
                         headerParameters,
                         parameters,
                         timeoutSeconds);

         return out;
      }

      boost::shared_ptr<CDataContainer> CHttpMethods::processJsonResponse(
         const std::map<std::string, std::string>& receivedHeaders,
         const std::string& data)
      {
         const auto& contentType = receivedHeaders.at("Content-Type");
         if (contentType.find("application/json") == std::string::npos &&
            contentType.find("text/json") == std::string::npos)
            YADOMS_LOG(warning) << "HTTP answer : JSON expected but content type not defined as JSON : " << contentType;

         // Content-Length is not always fulfilled so we don't use hasContentLength and getContentLength
         return CDataContainer::make(data);
      }
   }
} // namespace shared::http

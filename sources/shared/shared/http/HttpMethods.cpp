#include "stdafx.h"
#include "HttpMethods.h"
#include "HttpException.hpp"
#include <shared/Log.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>
#include <regex>

namespace shared
{
   class CCurlRessources
   {
   public:
      CCurlRessources()
      {
         cURLpp::initialize(CURL_GLOBAL_ALL);
      }

      virtual ~CCurlRessources()
      {
         cURLpp::terminate();
      }
   };

   // ReSharper disable once CppDeclaratorNeverUsed
   static CCurlRessources CurlRessources;

   std::string CHttpMethods::ProxyHost;
   int CHttpMethods::ProxyPort = kUseProxyDefaultPort;
   std::string CHttpMethods::ProxyUsername;
   std::string CHttpMethods::ProxyPassword;
   std::string CHttpMethods::ProxyBypassRegex;


   void CHttpMethods::setGlobalProxyConfig(const std::string& host,
                                           int port,
                                           const std::string& username,
                                           const std::string& password,
                                           const std::string& bypassRegex)
   {
      ProxyHost = host;
      ProxyPort = port;
      ProxyUsername = username;
      ProxyPassword = password;
      ProxyBypassRegex = bypassRegex;
   }

   std::string CHttpMethods::stringifyParameters(const std::map<std::string, std::string>& parameters)
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

      if (!ProxyHost.empty() &&
         (ProxyBypassRegex.empty() || !std::regex_search(url, std::regex(ProxyBypassRegex))))
      {
         request.setOpt(new curlpp::options::Proxy(ProxyHost));
         if (ProxyPort != kUseProxyDefaultPort)
            request.setOpt(new curlpp::options::ProxyPort(ProxyPort));
         if (!ProxyUsername.empty() && !ProxyPassword.empty())
            request.setOpt(new curlpp::options::ProxyUserPwd(ProxyUsername + ":" + ProxyPassword));
      }

      // URL + parameters
      request.setOpt(
         new curlpp::options::Url(url + stringifyParameters(parameters)));

      // Headers
      if (!headerParameters.empty())
      {
         std::list<std::string> headers;
         for (const auto& headerParametersIterator : headerParameters)
            headers.push_back(headerParametersIterator.first + ": " + headerParametersIterator.second);

         request.setOpt(new curlpp::options::HttpHeader(headers));
      }

      // Response headers
      char* headersBuffer = nullptr;
      size_t headersBufferSize = 0;
      request.setOpt(curlpp::options::HeaderFunction(
         [&headersBuffer, &headersBufferSize](char* ptr, size_t size, size_t nbItems)
         {
            const auto incomingSize = size * nbItems;
            headersBuffer = static_cast<char*>(realloc(headersBuffer, headersBufferSize + incomingSize));
            if (headersBuffer == nullptr)
               throw std::runtime_error("No enough memory");
            memcpy(&headersBuffer[headersBufferSize], ptr, incomingSize);
            headersBufferSize += incomingSize;
            return incomingSize;
         }));

      // Response data
      char* dataBuffer = nullptr;
      size_t dataBufferSize = 0;
      request.setOpt(curlpp::options::WriteFunction(
         [&dataBuffer, &dataBufferSize](char* ptr, size_t size, size_t nbItems)
         {
            const auto incomingSize = size * nbItems;
            dataBuffer = static_cast<char*>(realloc(dataBuffer, dataBufferSize + incomingSize));
            if (dataBuffer == nullptr)
               throw std::runtime_error("No enough memory");
            memcpy(&dataBuffer[dataBufferSize], ptr, incomingSize);
            dataBufferSize += incomingSize;
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
         throw CHttpException(message);
      }

      if (curlpp::infos::ResponseCode::get(request) != 200)
      {
         const auto message = (boost::format("Invalid HTTP result : %1%") % curlpp::infos::ResponseCode::get(request)
         ).str();
         YADOMS_LOG(warning) << message;
         throw CHttpException(message);
      }

      // Format headers
      std::vector<std::string> headerKeyValues;
      std::map<std::string, std::string> responseHeaders;
      split(headerKeyValues, headersBuffer, boost::is_any_of("\n"), boost::algorithm::token_compress_on);
      for (const auto& headerKeyValue : headerKeyValues)
      {
         const auto separatorIterator = headerKeyValue.find(':');
         if (separatorIterator == std::string::npos)
            continue;
         responseHeaders[headerKeyValue.substr(0, separatorIterator)] = headerKeyValue.substr(
            separatorIterator + 1, std::string::npos);
      }

      responseHandlerFct(responseHeaders,
                         std::string(dataBuffer, dataBufferSize));
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
      //TODO mettre du code en commun avec GET
      curlpp::Easy request;

      request.setOpt(new curlpp::options::PostFields(body));

      request.setOpt(new curlpp::options::Timeout(timeoutSeconds));

      if (!ProxyHost.empty() &&
         (ProxyBypassRegex.empty() || !std::regex_search(url, std::regex(ProxyBypassRegex))))
      {
         request.setOpt(new curlpp::options::Proxy(ProxyHost));
         if (ProxyPort != kUseProxyDefaultPort)
            request.setOpt(new curlpp::options::ProxyPort(ProxyPort));
         if (!ProxyUsername.empty() && !ProxyPassword.empty())
            request.setOpt(new curlpp::options::ProxyUserPwd(ProxyUsername + ":" + ProxyPassword));
      }

      // URL + parameters
      request.setOpt(
         new curlpp::options::Url(url + stringifyParameters(parameters)));

      // Headers
      if (!headerParameters.empty())
      {
         std::list<std::string> headers;
         for (const auto& headerParametersIterator : headerParameters)
            headers.push_back(headerParametersIterator.first + ": " + headerParametersIterator.second);

         request.setOpt(new curlpp::options::HttpHeader(headers));
      }

      // Response headers
      char* headersBuffer = nullptr;
      size_t headersBufferSize = 0;
      request.setOpt(curlpp::options::HeaderFunction(
         [&headersBuffer, &headersBufferSize](char* ptr, size_t size, size_t nbItems)
         {
            const auto incomingSize = size * nbItems;
            headersBuffer = static_cast<char*>(realloc(headersBuffer, headersBufferSize + incomingSize));
            if (headersBuffer == nullptr)
               throw std::runtime_error("No enough memory");
            memcpy(&headersBuffer[headersBufferSize], ptr, incomingSize);
            headersBufferSize += incomingSize;
            return incomingSize;
         }));

      // Response data
      char* dataBuffer = nullptr;
      size_t dataBufferSize = 0;
      request.setOpt(curlpp::options::WriteFunction(
         [&dataBuffer, &dataBufferSize](char* ptr, size_t size, size_t nbItems)
         {
            const auto incomingSize = size * nbItems;
            dataBuffer = static_cast<char*>(realloc(dataBuffer, dataBufferSize + incomingSize));
            if (dataBuffer == nullptr)
               throw std::runtime_error("No enough memory");
            memcpy(&dataBuffer[dataBufferSize], ptr, incomingSize);
            dataBufferSize += incomingSize;
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
         throw CHttpException(message);
      }

      if (curlpp::infos::ResponseCode::get(request) != 200 && curlpp::infos::ResponseCode::get(request) != 201)
      {
         const auto message = (boost::format("Invalid HTTP result : %1%") % curlpp::infos::ResponseCode::get(request)
         ).str();
         YADOMS_LOG(warning) << message;
         throw CHttpException(message);
      }

      // Format headers
      std::vector<std::string> headerKeyValues;
      std::map<std::string, std::string> responseHeaders;
      split(headerKeyValues, headersBuffer, boost::is_any_of("\n"), boost::algorithm::token_compress_on);
      for (const auto& headerKeyValue : headerKeyValues)
      {
         const auto separatorIterator = headerKeyValue.find(':');
         if (separatorIterator == std::string::npos)
            continue;
         responseHeaders[headerKeyValue.substr(0, separatorIterator)] = headerKeyValue.substr(
            separatorIterator + 1, std::string::npos);
      }

      responseHandlerFct(responseHeaders,
                         std::string(dataBuffer, dataBufferSize));
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
} // namespace shared

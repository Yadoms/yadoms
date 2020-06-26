#include "stdafx.h"
#include "curlppHelpers.h"
#include <regex>
#include <shared/Log.h>
#include "HttpMethods.h"
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>
#include <curlpp/cURLpp.hpp>
#include <shared/exception/HttpException.hpp>
#include "Proxy.h"

namespace shared
{
   namespace http
   {
      void CCurlppHelpers::setProxy(curlpp::Easy& request,
                                    const std::string& url,
                                    const std::string& proxyHost,
                                    int proxyPort,
                                    const std::string& proxyUsername,
                                    const std::string& proxyPassword,
                                    const std::string& proxyBypassRegex)
      {
         if (!proxyBypassRegex.empty() && std::regex_search(url, std::regex(proxyBypassRegex)))
            return;

         request.setOpt(new curlpp::options::Proxy(proxyHost));
         if (proxyPort != CProxy::kUseProxyDefaultPort)
            request.setOpt(new curlpp::options::ProxyPort(proxyPort));
         if (!proxyUsername.empty() && !proxyPassword.empty())
            request.setOpt(new curlpp::options::ProxyUserPwd(proxyUsername + ":" + proxyPassword));
      }

      void CCurlppHelpers::setHeaders(curlpp::Easy& request,
                                      const std::map<std::basic_string<char>, std::basic_string<char>>&
                                      headerParameters)
      {
         if (headerParameters.empty())
            return;

         std::list<std::string> headers;
         for (const auto& headerParametersIterator : headerParameters)
            headers.push_back(headerParametersIterator.first + ": " + headerParametersIterator.second);

         request.setOpt(new curlpp::options::HttpHeader(headers));
      }

      std::string CCurlppHelpers::stringifyParameters(const std::map<std::string, std::string>& parameters)
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

      void CCurlppHelpers::checkResult(const curlpp::Easy& request)
      {
         if (curlpp::infos::ResponseCode::get(request) != kHttpOk && curlpp::infos::
            ResponseCode::get(request) != kHttpCreated)
         {
            const auto message = (boost::format("Invalid HTTP result : %1%") % curlpp::infos::ResponseCode::get(request)
            ).str();
            YADOMS_LOG(warning) << message;
            throw exception::CHttpException(message);
         }
      }

      std::map<std::string, std::string> CCurlppHelpers::formatResponseHeaders(const std::string& headersBuffer)
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

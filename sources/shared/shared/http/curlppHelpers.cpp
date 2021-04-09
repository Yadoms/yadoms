#include "stdafx.h"
#include "curlppHelpers.h"
#include <regex>
#include <shared/Log.h>
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
                                    const std::string& url)
      {
         if (!CProxy::available())
            return;

         if (!CProxy::getBypassRegex().empty() && std::regex_search(url, std::regex(CProxy::getBypassRegex())))
            return;

         request.setOpt(new curlpp::options::Proxy(CProxy::getHost()));
         if (CProxy::getPort() != CProxy::kUseProxyDefaultPort)
            request.setOpt(new curlpp::options::ProxyPort(CProxy::getPort()));
         if (!CProxy::getUsername().empty() && !CProxy::getPassword().empty())
            request.setOpt(new curlpp::options::ProxyUserPwd(CProxy::getUsername() + ":" + CProxy::getPassword()));
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
         if (curlpp::infos::ResponseCode::get(request) != ECodes::kOKValue &&
            curlpp::infos::ResponseCode::get(request) != ECodes::kCreatedValue)
         {
            const auto message = (boost::format("Invalid HTTP result : %1%") %
               curlpp::infos::ResponseCode::get(request)).str();
            YADOMS_LOG(warning) << message;
            throw exception::CHttpException(message, ECodes(curlpp::infos::ResponseCode::get(request)));
         }
      }
   }
} // namespace shared::http

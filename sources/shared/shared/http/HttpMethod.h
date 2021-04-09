#pragma once
#include <map>
#include <functional>
#include <string>

#include <curlpp/Easy.hpp>


namespace shared
{
   namespace http
   {
      //--------------------------------------------------------------
      /// \brief	Helpers fot HTTP(s) requests
      //--------------------------------------------------------------
      class CHttpMethod final //TODO renommer (CCurlppHttpRequest ?) //TODO faire une interface et une factory (abstraire curl aux usagers)
      {
      public:
         //--------------------------------------------------------------
         /// \brief	default value for HTTP Request default timeout
         //--------------------------------------------------------------
         static const int HttpRequestDefaultTimeoutSeconds = 45;

         enum class ERequest
         {
            kGet = 0,
            kPut,
            kPost,
            kHead
         };

         CHttpMethod(ERequest requestType,
                     std::string url);

         ~CHttpMethod() = default;

         CHttpMethod& withHeaderParameters(const std::map<std::string, std::string>& headerParameters);
         CHttpMethod& withParameters(const std::map<std::string, std::string>& parameters);
         CHttpMethod& withTimeout(int timeoutSeconds);
         CHttpMethod& withBody(const std::string& body);

         void send(const std::function<void(const std::map<std::string, std::string>& receivedHeaders,
                                            const std::string& data)>& responseHandlerFct);

      private:
         void setProxy(const std::string& url,
                       const std::string& proxyHost,
                       int proxyPort,
                       const std::string& proxyUsername,
                       const std::string& proxyPassword,
                       const std::string& proxyBypassRegex);
         static std::string stringifyParameters(const std::map<std::string, std::string>& parameters);
         void setHeaders(const std::map<std::basic_string<char>, std::basic_string<char>>& headerParameters);
         void checkResult() const;
         std::map<std::string, std::string> formatResponseHeaders(const std::string& headersBuffer) const;

         curlpp::Easy m_request;
         ERequest m_requestType;
         const std::string m_url;
         int m_timeoutSeconds;
         std::map<std::string, std::string> m_headerParameters;
         std::map<std::string, std::string> m_parameters;
         std::string m_body;
      };
   }
} // namespace shared::http

#pragma once
#include <curlpp/Easy.hpp>


namespace shared
{
   namespace http
   {
      //--------------------------------------------------------------
      /// \brief	Helpers for cCURLpp use
      //--------------------------------------------------------------
      class CCurlppHelpers
      {
      public:
         CCurlppHelpers() = delete;
         virtual ~CCurlppHelpers() = default;

         static void setProxy(curlpp::Easy& request,
                              const std::string& url);

         static void setHeaders(curlpp::Easy& request,
                                const std::map<std::basic_string<char>, std::basic_string<char>>& headerParameters);

         static std::string stringifyParameters(const std::map<std::string, std::string>& parameters);

         static void checkResult(const curlpp::Easy& request);
      };
   }
} // namespace shared::http

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
			                     const std::string& url,
			                     const std::string& proxyHost,
			                     int proxyPort,
			                     const std::string& proxyUsername,
			                     const std::string& proxyPassword,
			                     const std::string& proxyBypassRegex);

			static void setHeaders(curlpp::Easy& request,
			                       const std::map<std::basic_string<char>, std::basic_string<char>>& headerParameters);

			static std::string stringifyParameters(const std::map<std::string, std::string>& parameters);

			static void checkResult(const curlpp::Easy& request);

			static std::map<std::string, std::string> formatResponseHeaders(const std::string& headersBuffer);

			enum EHttpCodeStatus
			{
				kHttpOk = 200,
				kHttpCreated= 201,
				kHttpBadRequest=400,
				kHttpUnauthorized = 401,
				kHttpForbidden= 403,
				kHttpNotFound = 404,
			};
		};
	}
} // namespace shared::http

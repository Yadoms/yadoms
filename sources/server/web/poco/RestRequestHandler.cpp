#include "stdafx.h"
#include "RestRequestHandler.h"
#include <shared/Log.h>
#include "web/rest/Result.h"
#include <boost/algorithm/string.hpp>
#include <Poco/URI.h>
#include <string>
#include <string_view>
#include <vector>

#include <utility>

namespace web
{
	namespace poco
	{
		CRestRequestHandler::CRestRequestHandler(std::string restBaseKeyword,
												 const std::vector<boost::shared_ptr<rest::service::IRestService>>& services)
			: m_restBaseKeyword(std::move(restBaseKeyword))
		{
			for (auto& service : services)
				CRestRequestHandler::registerRestService(service);
			CRestRequestHandler::initialize();
		}

		const std::string& CRestRequestHandler::getRestKeyword() const
		{
			return m_restBaseKeyword;
		}


		std::vector<std::string> CRestRequestHandler::splitAnyOfAndCompress(std::string_view input,
																			std::string_view seps)
		{
			std::vector<std::string> result;
			std::string token;

			auto is_sep = [&](char c) {
				return seps.find(c) != std::string_view::npos;
			};

			for (char c : input)
			{
				if (is_sep(c))
				{
					if (!token.empty())            // compression => ignore vides
					{
						result.push_back(std::move(token));
						token.clear();
					}
				}
				else
				{
					token += c;
				}
			}

			if (!token.empty())
				result.push_back(std::move(token));

			return result;
		}


		std::vector<std::string> CRestRequestHandler::parseUrl(const std::string& url)
		{
			std::vector<std::string> results;
			//split on slash or anti slash
			auto strings = splitAnyOfAndCompress(url, "/\\");
			//remove empty strings
			//do not use std::empty in std::remove_if because MacOs Clang do not support it
			auto i = strings.begin();
			while (i != strings.end())
			{
				if (i->empty())
				{
					i = strings.erase(i);
				}
				else
				{
					//each url parameter must be url decoded
					std::string decodedString;
					Poco::URI::decode(*i, decodedString);
					results.push_back(decodedString);

					++i;
				}
			}

			return results;
		}

		std::string CRestRequestHandler::manageRestRequests(Poco::Net::HTTPServerRequest& request)
		{
			// Decode url to path.
			auto requestPath = request.getURI();

			try
			{
				//remove the fist /rest/ string
				requestPath = requestPath.substr(m_restBaseKeyword.size());

				//parse url to parameters
				const auto parameters = parseUrl(requestPath);

				std::string content;
				if (request.hasContentLength())
				{
					content.resize(static_cast<unsigned int>(request.getContentLength()));
					request.stream().read(const_cast<char*>(content.c_str()), request.getContentLength());
				}

				//dispatch url to rest dispatcher
				const auto js = m_restDispatcher.dispatch(request.getMethod(), parameters, content);
				auto temp = js->serialize();
				return temp;
			}
			catch (std::exception& ex)
			{
				YADOMS_LOG(error) << "An exception occured in treating REST url : " << requestPath << std::endl << "Exception : " << ex.what();
				return rest::CResult::GenerateError(ex)->serialize();
			}
			catch (...)
			{
				YADOMS_LOG(error) << "An unknown exception occured in treating REST url : " << requestPath;
				return rest::CResult::GenerateError("An unknown exception occured in treating REST url : " + requestPath)->serialize();
			}
		}

		void CRestRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
												Poco::Net::HTTPServerResponse& response)
		{
			YADOMS_LOG(trace) << "Rest request : [" << request.getMethod() << "] : " << request.getURI();

			const auto answer = manageRestRequests(request);
			response.setContentType("application/json; charset=utf-8");
			auto& stream = response.send();
			stream << answer;
		}

		void CRestRequestHandler::registerRestService(boost::shared_ptr<rest::service::IRestService> restService)
		{
			if (restService.get() != nullptr)
				m_restService.push_back(restService);
		}

		void CRestRequestHandler::initialize()
		{
			for (auto& i : m_restService)
			{
				if (i.get() != nullptr)
					i->configureDispatcher(m_restDispatcher);
			}
		}
	} //namespace poco
} //namespace web

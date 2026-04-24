#include "stdafx.h"
#include "WebsiteRequestHandler.h"
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPBasicCredentials.h>
#include "MimeType.h"
#include <shared/encryption/Md5.h>
#include <string>
#include <algorithm>
#include <cctype>

namespace web::poco
{
	CWebsiteRequestHandler::CWebsiteRequestHandler(const std::string& documentRoot, const std::map<std::string, std::string>& alias)
		:m_documentRoot(documentRoot)
	{
		std::map<std::string, std::string>::const_iterator i;
		for (i = alias.begin(); i != alias.end(); ++i)
			configureAlias(i->first, i->second);
	}

	CWebsiteRequestHandler::~CWebsiteRequestHandler()
	{
	}

	static bool istarts_with(const std::string& str, const std::string& prefix)
	{
		if (prefix.size() > str.size())
			return false;

		return std::equal(
			prefix.begin(), prefix.end(),
			str.begin(),
			[](char a, char b) {
				return std::tolower(static_cast<unsigned char>(a)) ==
					std::tolower(static_cast<unsigned char>(b));
			}
		);
	}

	static bool iends_with(const std::string& str, const std::string& suffix)
	{
		if (suffix.size() > str.size())
			return false;

		return std::equal(
			suffix.rbegin(), suffix.rend(),
			str.rbegin(),
			[](char a, char b) {
				return std::tolower(static_cast<unsigned char>(a)) ==
					std::tolower(static_cast<unsigned char>(b));
			}
		);
	}

	void CWebsiteRequestHandler::configureAlias(const std::string& alias, const std::string& documentsPath)
	{
		if (boost::filesystem::is_directory(boost::filesystem::path(documentsPath)))
		{
			std::string realAlias = alias;
			if (!istarts_with(realAlias, "/"))
				realAlias = "/" + realAlias;
			if (!iends_with(realAlias, "/"))
				realAlias = realAlias + "/";

			std::string realDocumentsPath = documentsPath;
			if (!iends_with(realDocumentsPath, "/"))
				realDocumentsPath = realDocumentsPath + "/";

			m_alias[realAlias] = realDocumentsPath;
		}
		else
		{
			std::string realAlias = alias;
			if (!istarts_with(realAlias, "/"))
				realAlias = "/" + realAlias;
			m_alias[realAlias] = documentsPath;
		}
	}

	bool CWebsiteRequestHandler::readAndSendFile(std::string& fullpath, Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
	{
		// Determine the file extension.
		std::size_t last_slash_pos = fullpath.find_last_of("/");
		std::size_t last_dot_pos = fullpath.find_last_of(".");
		std::string extension;
		if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
		{
			extension = fullpath.substr(last_dot_pos + 1);
		}

		if (boost::filesystem::exists(fullpath) && m_cacheManager.checkCache(fullpath, request, response) == CWebsiteCacheManager::kNothingChanged)
		{
			//etag has not changed, so just answer "not modified"
			//(the browser is allowed to use local file from its cache and update expiration date)
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_MODIFIED);
			response.send();
			return true;
		}
		else
		{
			std::ifstream is(fullpath.c_str(), std::ios::in | std::ios::binary);
			if (is)
			{
				response.setChunkedTransferEncoding(true);
               response.setContentType(CMimeType::extensionToType(extension));
				std::ostream& ostr = response.send();
				char buf[512];
				while (is.read(buf, sizeof(buf)).gcount() > 0)
					ostr.write(buf, (unsigned int)is.gcount());
				return true;
			}
			return false;
		}
	}

	void CWebsiteRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
	{
		std::string uri = request.getURI();

		//extract the real uri (without url parameters, : index.html?param=value....
		std::size_t last_interrogation_pos = uri.find_last_of("?");
		if (last_interrogation_pos != std::string::npos)
			uri = uri.substr(0, last_interrogation_pos);

		// If path ends in slash (i.e. is a directory) then add "index.html".
		if (uri[uri.size() - 1] == '/')
		{
			uri += "index.html";
		}

		// Open the file to send back.
		std::string full_path = m_documentRoot + uri;

		std::map<std::string, std::string>::iterator i;
		for (i = m_alias.begin(); i != m_alias.end(); ++i)
		{
			if (istarts_with(uri, i->first))
			{
				std::string repl = uri;
				repl.replace(0, i->first.size(), i->second);

				full_path = repl;
			}
		}

		//try to send file
		if (!readAndSendFile(full_path, request, response))
		{
			//the 404.html has not been found, just return a 404 hard coded message
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);

			std::string html404 = m_documentRoot + "/404.html";
			if (!readAndSendFile(html404, request, response))
			{
				//the file is not found, try to send 404.html file
				response.setContentType("text/html");
				response.setChunkedTransferEncoding(false);

				std::stringstream resp_body;
				resp_body << "<html><head><title>404 Not Found</title>";
				resp_body << "</head>";
				resp_body << "<body><p style=\"font-size: 24;\">Yadoms Web Service</p>";
				resp_body << "<p style=\"font-size: 24;\"><H1>Not Found</H1></p><p style=\"font-size: 24;\">";
				resp_body << "The requested URL " << request.getURI() << " was not found.";
				resp_body << "</p></body></html>";
				response.setContentLength(resp_body.str().length());
				std::ostream& ostr = response.send();
				ostr << resp_body.str();
			}
		}

	}
}



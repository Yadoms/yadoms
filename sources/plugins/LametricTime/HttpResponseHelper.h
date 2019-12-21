#pragma once
#include <string>

class CHttpResponseHelper
{
public:
	static std::string getHttpResponseHeaderField(const std::string& headerFieldName,
	                                              const std::string& httpResponse);
	static bool isValidHttpResponse(const std::string& httpResponse);
	static bool isValidHttpStatusCode(const std::string& httpResponse);
};

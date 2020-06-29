#pragma once
#include <string>

namespace shared
{
   namespace http
   {
      namespace ssdp
      {
         class CHttpResponseHelper //TODO utile ?
         {
         public:
            static std::string getHttpResponseHeaderField(const std::string& headerFieldName,
                                                          const std::string& httpResponse);
            static bool isValidHttpResponse(const std::string& httpResponse);
            static bool isValidHttpStatusCode(const std::string& httpResponse);
            static int getHttpStatusCode(std::string& input);

            enum EHttpCodeStatus //TODO utile ?
            {
               kHttpOk = 200,
               kHttpCreated = 201,
               kHttpUnauthorized = 401,
            };
         };
      }
   }
}

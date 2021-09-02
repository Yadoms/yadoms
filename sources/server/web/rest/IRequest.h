#pragma once
#include <shared/http/Codes.h>
#include "ContentType.h"
#include "shared/http/HttpRestVerb.h"

namespace web
{
   namespace rest
   {
      class IRequest
      {
      public:
         virtual ~IRequest() = default;

         virtual shared::http::ERestVerb method() = 0;

         virtual std::string parameter(const std::string& key) = 0;
         virtual std::string parameter(const std::string& key,
                                       const std::string& defaultValue) = 0;
         virtual std::map<std::string, std::string> parameters() = 0;

         virtual EContentType contentType() = 0;
         virtual std::string body() = 0;

         /// @brief Check if content type is accepted for answer
         /// @param contentType content type to test
         /// @return quality factor (see https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Accept)
         virtual float acceptContentType(EContentType contentType) = 0;
      };
   } //namespace rest
} //namespace web 

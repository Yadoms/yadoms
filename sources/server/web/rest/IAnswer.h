#pragma once
#include <shared/http/Codes.h>
#include "ContentType.h"

namespace web
{
   namespace rest
   {
      class IAnswer
      {
      public:
         virtual ~IAnswer() = default;

         virtual shared::http::ECodes code() const = 0;
         virtual std::string body() const = 0;
         virtual bool bodyIsFile() const = 0;
         virtual EContentType contentType() const = 0;
         virtual std::shared_ptr<std::map<std::string, std::string>> customHeaders() const = 0;
      };
   } //namespace rest
} //namespace web 
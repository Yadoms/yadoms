#pragma once
#include <shared/http/Codes.h>
#include "ContentType.h"
#include "service/SseConnectionHandler.h"

namespace web
{
   namespace rest
   {
      class IAnswer
      {
      public:
         enum class EBodyType
         {
            kString,
            kFile,
            kStream
         };

         virtual ~IAnswer() = default;

         virtual shared::http::ECodes code() const = 0;
         virtual std::string body() const = 0;
         virtual EBodyType bodyType() const = 0;
         virtual EContentType contentType() const = 0;
         virtual std::shared_ptr<std::map<std::string, std::string>> customHeaders() const = 0;
         virtual std::shared_ptr<service::ISseConnectionHandler> connectionHandler() const = 0;
      };
   } //namespace rest
} //namespace web 

#pragma once
#include <shared/event/EventHandler.hpp>
#include <shared/http/Codes.h>
#include "ContentType.h"

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
         virtual boost::shared_ptr<shared::event::CEventHandler> streamingEventHandler() const = 0;
         virtual int streamingOnNewEventId() const = 0;
         virtual EBodyType bodyType() const = 0;
         virtual EContentType contentType() const = 0;
         virtual std::shared_ptr<std::map<std::string, std::string>> customHeaders() const = 0;
      };
   } //namespace rest
} //namespace web 

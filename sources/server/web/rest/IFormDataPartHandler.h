#pragma once
#include "ContentType.h"

namespace web::rest
{
   class IFormDataPartHandler
   {
   public:
      virtual ~IFormDataPartHandler() = default;

      virtual EContentType contentType() const = 0;
   };
}

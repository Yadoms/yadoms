#pragma once
#include "ContentType.h"

namespace web
{
   namespace rest
   {
      class IFormDataPartHandler
      {
      public:
         virtual ~IFormDataPartHandler() = default;

         virtual EContentType contentType() const = 0;
      };
   } //namespace rest
} //namespace web 

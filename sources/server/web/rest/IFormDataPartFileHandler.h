#pragma once
#include "IFormDataPartHandler.h"

namespace web
{
   namespace rest
   {
      class IFormDataPartFileHandler : public IFormDataPartHandler
      {
      public:
         ~IFormDataPartFileHandler() override = default;

         virtual long long fileSize() const = 0; // Returns -1 if unknown
         virtual std::string fileName() const = 0;
      };
   } //namespace rest
} //namespace web 

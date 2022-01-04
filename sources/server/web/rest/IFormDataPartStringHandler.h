#pragma once
#include "IFormDataPartHandler.h"

namespace web
{
   namespace rest
   {
      class IFormDataPartStringHandler : public IFormDataPartHandler
      {
      public:
         ~IFormDataPartStringHandler() override = default;

         virtual std::string string() const = 0;
      };
   } //namespace rest
} //namespace web 

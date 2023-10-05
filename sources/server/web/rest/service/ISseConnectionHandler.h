#pragma once
#include <string>

namespace web
{
   namespace rest
   {
      namespace service
      {
         class ISseConnectionHandler
         {
         public:
            virtual ~ISseConnectionHandler() = default;

            virtual std::string handle() const = 0;
         };
      }
   } //namespace oatppServer
} //namespace web

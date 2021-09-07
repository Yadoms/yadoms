#pragma once

namespace web
{
   namespace poco
   {
      class CMimeType
      {
      public:
         static std::string extensionToType(const std::string& extension);
      };
   } //namespace poco
} //namespace web

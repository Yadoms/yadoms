#pragma once

namespace web { namespace poco {

   class CMimeType
   {
   public:
      static std::string extension_to_type(const std::string& extension);
   };


} //namespace poco
} //namespace web


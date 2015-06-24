#pragma once

#include <shared/Export.h>

namespace shared { namespace encryption { 

   //--------------------------------------------------------------
   /// \brief	Base64 encryption/decryption
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CBase64
   {
   public:
      static std::string encode(unsigned char const* , unsigned int len);
      static std::string decode(std::string const& s);
      
      static inline bool is_base64(unsigned char c);
      
   private:
      static std::string m_base64Chars;
   };

} // namespace encryption
} // namespace shared
#include "stdafx.h"
#include "Xor.h"
#include "Base64.h"

namespace shared { namespace encryption { 

   const std::string CXor::InternalKey = "2m72fgEQ";

   std::string CXor::encryptBase64(const std::string & toEncrypt)
   {
      return encryptBase64(toEncrypt, InternalKey);
   }

   std::string CXor::decryptBase64(const std::string & toDecrypt)
   {
      return decryptBase64(toDecrypt, InternalKey);
   }

   std::string CXor::encryptBase64(const std::string & toEncrypt, const std::string & key)
   {
      std::string cypher = encryptDecrypt(toEncrypt, key);
      return CBase64::encode((const unsigned char*)cypher.c_str(), cypher.size());   
   }

   std::string CXor::decryptBase64(const std::string & toDecrypt, const std::string & key)
   {
      return encryptDecrypt(CBase64::decode(toDecrypt), key);
   }

   std::string CXor::encryptDecrypt(const std::string & toEncryptOrDecrypt, const std::string & key)
   {
      std::string output = toEncryptOrDecrypt; //set the output size
      for (unsigned int i = 0; i < toEncryptOrDecrypt.size(); i++)
         output[i] = toEncryptOrDecrypt[i] ^ key[i % key.size()];
      return output;
   }
  
   
} // namespace encryption
} // namespace shared

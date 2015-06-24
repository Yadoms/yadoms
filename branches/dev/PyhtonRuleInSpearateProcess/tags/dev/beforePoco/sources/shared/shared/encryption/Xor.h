#pragma once

#include <shared/Export.h>

namespace shared { namespace encryption { 

   //--------------------------------------------------------------
   /// \brief	XOR encryption/decryption
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CXor
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Encrypt a string using XOR algorithm, with a base64 pass
      /// \param [in] toEncrypt  The string to encrypt
      /// \param [in] key        The encryption key
      /// \return	The encrypted string (base64)
      //--------------------------------------------------------------
      static std::string encryptBase64(const std::string & toEncrypt, const std::string & key);
      
      //--------------------------------------------------------------
      /// \brief	Decrypt a base64 string using XOR algorithm
      /// \param [in] toDecrypt  The base64 string to decrypt
      /// \param [in] key        The decryption key
      /// \return	The decrypted string
      //--------------------------------------------------------------
      static std::string decryptBase64(const std::string & toDecrypt, const std::string & key);
      
   private:
      //--------------------------------------------------------------
      /// \brief	Encrypt or decrypt a string using XOR algorithm
      /// \param [in] toEncryptOrDecrypt  The string to encrypt or decrypt
      /// \param [in] key                 The encryption/decryption key
      /// \return	The encrypted/decrypted string
      //--------------------------------------------------------------
      static std::string encryptDecrypt(const std::string & toEncryptOrDecrypt, const std::string & key);

   };

} // namespace encryption
} // namespace shared
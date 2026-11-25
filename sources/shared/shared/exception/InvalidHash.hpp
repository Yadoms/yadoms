#pragma once
#include "Exception.hpp"

namespace shared::exception
{
   //--------------------------------------------------------------
   /// \brief Exception for invalid Hash
   //--------------------------------------------------------------
   class CInvalidHash : public CException
   {
   public:
      explicit CInvalidHash(const boost::filesystem::path& fileChecked)
         : CException(std::string("Invalid MD5 hash for  : " + fileChecked.string()))
      {
      }

      CInvalidHash(const boost::filesystem::path& fileChecked,
                   const std::string& expectedHash,
                   const std::string& computedHash)
         : CException(std::string(
            "Invalid MD5 hash for  : " + fileChecked.string() + " Expected : " + expectedHash + " Computed : " +
            computedHash))
      {
      }

      ~CInvalidHash() noexcept override = default;
   };
}

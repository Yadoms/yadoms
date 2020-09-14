#pragma once
#include "Exception.hpp"

namespace shared
{
   namespace exception
   {
      //--------------------------------------------------------------
      /// \brief Exception for invalid Hash
      //--------------------------------------------------------------
      class CInvalidHash : public shared::exception::CException
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

         virtual ~CInvalidHash() noexcept = default;
      };
   }
} // namespace shared::exception

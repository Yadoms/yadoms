#pragma once

#include <shared/exception/Exception.hpp>

namespace shared { namespace web { namespace exception 
{

   //--------------------------------------------------------------
   /// \class Exception for invalid URI
   //--------------------------------------------------------------
   class CInvalidHash : public shared::exception::CException
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      explicit CInvalidHash(const Poco::Path & fileChecked)
         :shared::exception::CException(std::string("Invalid MD5 hash for  : " + fileChecked.toString()))
      {
      }   

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CInvalidHash(const Poco::Path & fileChecked, const std::string & expectedHash, const std::string & computedHash)
         :shared::exception::CException(std::string("Invalid MD5 hash for  : " + fileChecked.toString() + " Expected : " + expectedHash + " Computed : " + computedHash))
      {
      } 


      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CInvalidHash() throw()
      {
      }
   };

} } } // namespace shared::web::exception

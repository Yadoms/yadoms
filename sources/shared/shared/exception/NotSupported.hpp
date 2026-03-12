#pragma once
#include "Exception.hpp"

namespace shared::exception
{
   //--------------------------------------------------------------
   /// \brief Exception for non supported function
   //--------------------------------------------------------------
   class CNotSupported : public CException
   {
   public:
      explicit CNotSupported(const std::string& function)
         : CException(function + " is not supported")
      {
      }

      ~CNotSupported() throw() override = default;
   };

   // Helper macro
#define NOT_SUPPORTED    throw CNotSupported(__FUNCTION__)
}

#pragma once
#include "Exception.hpp"


namespace shared::exception
{
   //--------------------------------------------------------------
   /// \brief Exception for non implemented part of code
   //--------------------------------------------------------------
   class CNotImplemented : public CException
   {
   public:
      explicit CNotImplemented(const std::string& function)
         : CException(function + " is not yet implemented")
      {
      }

      ~CNotImplemented() throw() override = default;
   };
}

// Helper macro
#define NOT_IMPLEMENTED    throw shared::exception::CNotImplemented(__FUNCTION__)

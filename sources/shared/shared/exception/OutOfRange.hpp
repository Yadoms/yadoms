#pragma once

#include "Exception.hpp"

namespace shared::exception
{
   //--------------------------------------------------------------
   /// \brief Exception for value out of range
   //--------------------------------------------------------------
   class COutOfRange : public CException
   {
   public:
      explicit COutOfRange(const std::string& message)
         : CException(std::string("Out of range " + message))
      {
      }

      ~COutOfRange() throw() override = default;
   };
}

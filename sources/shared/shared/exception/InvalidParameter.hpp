#pragma once

#include "Exception.hpp"

namespace shared::exception
{
   //--------------------------------------------------------------
   /// \brief Exception for accessing NULL objects
   //--------------------------------------------------------------
   class CInvalidParameter : public CException
   {
   public:
      explicit CInvalidParameter(const std::string& parameter)
         : CException(std::string("Invalid parameter " + parameter))
      {
      }

      ~CInvalidParameter() throw() override = default;
   };
}

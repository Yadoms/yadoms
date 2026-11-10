#pragma once

#include "Exception.hpp"

namespace shared::exception
{
   //--------------------------------------------------------------
   /// \brief Exception when archive extraction fails
   //--------------------------------------------------------------
   class CExtract : public CException
   {
   public:
      explicit CExtract(const std::string& message)
         : CException(message)
      {
      }

      ~CExtract() throw() override
      {
      }
   };
}

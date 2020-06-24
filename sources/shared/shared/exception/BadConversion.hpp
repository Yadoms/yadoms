#pragma once

#include "Exception.hpp"

namespace shared
{
   namespace exception
   {
      //--------------------------------------------------------------
      /// \brief Exception for fail to cast object
      //--------------------------------------------------------------
      class CBadConversion : public CException
      {
      public:
         //--------------------------------------------------------------
         /// \brief	                        Constructor
         //--------------------------------------------------------------
         CBadConversion(const std::string& variableName, const std::string& variableType)
            : CException(std::string("Bad conversion of " + variableName + " to " + variableType))
         {
         }


         //--------------------------------------------------------------
         /// \brief      Destructor
         //--------------------------------------------------------------
         virtual ~CBadConversion() throw()
         {
         }
      };
   }
} // namespace shared::exception

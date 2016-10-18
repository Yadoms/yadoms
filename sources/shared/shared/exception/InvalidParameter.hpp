#pragma once

#include "Exception.hpp"

namespace shared
{
   namespace exception
   {
      //--------------------------------------------------------------
      /// \brief Exception for accessing NULL objects
      //--------------------------------------------------------------
      class CInvalidParameter : public CException
      {
      public:
         //--------------------------------------------------------------
         /// \brief	                        Constructor
         //--------------------------------------------------------------
         explicit CInvalidParameter(const std::string& parameter)
            : CException(std::string("Invalid parameter " + parameter))
         {
         }

         //--------------------------------------------------------------
         /// \brief      Destructor
         //--------------------------------------------------------------
         virtual ~CInvalidParameter() throw()
         {
         }
      };
   }
} // namespace shared::exception

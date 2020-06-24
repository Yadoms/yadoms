#pragma once

#include "Exception.hpp"

namespace shared
{
   namespace exception
   {
      //--------------------------------------------------------------
      /// \brief Exception for accessing NULL objects
      //--------------------------------------------------------------
      class CParse : public CException
      {
      public:
         //--------------------------------------------------------------
         /// \brief	                        Constructor
         //--------------------------------------------------------------
         explicit CParse(const std::string& message)
            : CException(message)
         {
         }

         //--------------------------------------------------------------
         /// \brief      Destructor
         //--------------------------------------------------------------
         virtual ~CParse() throw()
         {
         }
      };
   }
} // namespace shared::exception

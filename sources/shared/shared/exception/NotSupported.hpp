#pragma once
#include "Exception.hpp"

namespace shared
{
   namespace exception
   {
      //--------------------------------------------------------------
      /// \brief Exception for non supported function
      //--------------------------------------------------------------
      class CNotSupported : public CException
      {
      public:
         //--------------------------------------------------------------
         /// \brief	                        Constructor
         //--------------------------------------------------------------
         explicit CNotSupported(const std::string& function)
            : CException(function + " is not supported")
         {
         }

         //--------------------------------------------------------------
         /// \brief      Destructor
         //--------------------------------------------------------------
         virtual ~CNotSupported() throw()
         {
         }
      };

      // Helper macro
#define NOT_SUPPORTED    throw CNotSupported(__FUNCTION__)
   }
} // namespace shared::exception

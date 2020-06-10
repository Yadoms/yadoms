#pragma once
#include "Exception.hpp"

namespace shared
{
   namespace exception
   {
      //--------------------------------------------------------------
      /// \brief	Exception handling for protocol errors
      //--------------------------------------------------------------
      class CHttpException : public CException
      {
      public:
         //--------------------------------------------------------------
         /// \brief	                        Constructor
         /// \param[in]  message             Exception message
         //--------------------------------------------------------------
         explicit CHttpException(const std::string& message)
            : CException(message)
         {
         }

         virtual ~CHttpException() throw() = default;
      };
   }
};

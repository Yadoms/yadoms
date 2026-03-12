#pragma once
#include "Exception.hpp"
#include "shared/http/Codes.h"

namespace shared::exception
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
         /// \param[in]  httpCode            HTTP code
      //--------------------------------------------------------------
      CHttpException(const std::string& message,
                     const http::ECodes& httpCode)
         : CException(message),
           m_httpCode(httpCode)
      {
      }

      const http::ECodes& code() const
      {
         return m_httpCode;
      }

      ~CHttpException() override = default;

   private:
      const http::ECodes m_httpCode;
   };
};

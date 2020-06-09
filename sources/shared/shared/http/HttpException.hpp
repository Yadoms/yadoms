#pragma once
#include <shared/exception/Exception.hpp>

namespace shared
{
//--------------------------------------------------------------
/// \brief	Exception handling for protocol errors
//--------------------------------------------------------------
   class CHttpException : public shared::exception::CException
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in]  message             Exception message
      //--------------------------------------------------------------
      explicit CHttpException(const std::string& message)
         :CException(message)
      {
      }

      virtual ~CHttpException() throw() = default;
   };
};

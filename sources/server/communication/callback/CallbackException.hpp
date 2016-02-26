#pragma once
#include <shared/exception/Exception.hpp>

namespace communication { namespace callback {

   //--------------------------------------------------------------
   /// \brief	Exception handling for protocol errors
   //--------------------------------------------------------------
   class CCallbackException : public shared::exception::CException
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in]  message             Exception message
      //--------------------------------------------------------------
      explicit CCallbackException(const std::string& message)
         :CException(message)
      {
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CCallbackException() throw()
      {
      }
   };

}
} // namespace communication::callback

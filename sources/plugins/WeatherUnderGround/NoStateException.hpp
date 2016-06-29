#pragma once
#include <shared/exception/Exception.hpp>

//--------------------------------------------------------------
/// \brief	Exception handling for protocol errors
//--------------------------------------------------------------
class CNoStateException : public shared::exception::CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in]  message             Exception message
   //--------------------------------------------------------------
   explicit CNoStateException(const std::string& message)
      : CException(message)
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CNoStateException() throw()
   {
   }
};


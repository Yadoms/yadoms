#pragma once
#include <shared/exception/Exception.hpp>

//--------------------------------------------------------------
/// \brief	Exception handling for protocol errors
//--------------------------------------------------------------
class CRequestErrorException : public shared::exception::CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in]  message             Exception message
   //--------------------------------------------------------------
   explicit CRequestErrorException()
      : CException("Fail to send url")
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CRequestErrorException() throw()
   {
   }
};


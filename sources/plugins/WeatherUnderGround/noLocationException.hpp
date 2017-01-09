#pragma once
#include <shared/exception/Exception.hpp>

//--------------------------------------------------------------
/// \brief	Exception handling for protocol errors
//--------------------------------------------------------------
class CNoLocationException : public shared::exception::CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in]  message             Exception message
   //--------------------------------------------------------------
   explicit CNoLocationException()
      : CException("No location defined !")
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CNoLocationException() throw()
   {
   }
};


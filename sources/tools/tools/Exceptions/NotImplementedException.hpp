#pragma once
#include "Exception.hpp"

//--------------------------------------------------------------
/// \class Exception for non implemented part of code
//--------------------------------------------------------------
class CNotImplementedException : public CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   //--------------------------------------------------------------
   CNotImplementedException()
      :CException("Not yet implemented")
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CNotImplementedException() throw()
   {
   }
};
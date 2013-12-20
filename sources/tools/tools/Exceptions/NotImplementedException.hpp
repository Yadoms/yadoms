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
   CNotImplementedException(const std::string& function)
      :CException(function + " is not yet implemented")
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CNotImplementedException() throw()
   {
   }
};

// Helper macro
#define NOT_IMPLEMENTED    throw CNotImplementedException(__FUNCTION__)

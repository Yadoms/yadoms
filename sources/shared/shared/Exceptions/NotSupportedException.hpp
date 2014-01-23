#pragma once
#include "Exception.hpp"

//--------------------------------------------------------------
/// \class Exception for non supported function
//--------------------------------------------------------------
class CNotSupportedException : public CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   //--------------------------------------------------------------
   CNotSupportedException(const std::string& function)
      :CException(function + " is not supported")
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CNotSupportedException() throw()
   {
   }
};

// Helper macro
#define NOT_SUPPORTED    throw CNotSupportedException(__FUNCTION__)

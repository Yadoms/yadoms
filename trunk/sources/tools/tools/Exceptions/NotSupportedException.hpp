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
      :CException(std::string(function + "is not supported"))
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CNotSupportedException() throw()
   {
   }
};
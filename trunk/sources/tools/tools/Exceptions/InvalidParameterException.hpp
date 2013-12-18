#pragma once

#include "Exception.hpp"

//--------------------------------------------------------------
/// \class Exception for accessing NULL objects
//--------------------------------------------------------------
class CInvalidParameterException : public CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   //--------------------------------------------------------------
   CInvalidParameterException(const std::string& parameter)
      :CException(std::string("Invalid parameter " + parameter))
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CInvalidParameterException() throw()
   {
   }
};

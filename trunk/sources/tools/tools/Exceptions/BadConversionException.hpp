#pragma once

#include "Exception.hpp"

//--------------------------------------------------------------
/// \class Exception for fail to cast object
//--------------------------------------------------------------
class CBadConversionException : public CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   //--------------------------------------------------------------
   CBadConversionException(const std::string& variableName, const std::string& variableType)
      :CException(std::string("Bad conversion of " + variableName + " to " + variableType))
   {
   }


   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CBadConversionException() throw()
   {
   }
};

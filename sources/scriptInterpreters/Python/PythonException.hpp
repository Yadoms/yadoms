#pragma once

#include <shared/exception/Exception.hpp>

//--------------------------------------------------------------
/// \class Exception raised by Python error
//--------------------------------------------------------------
class CPythonException : public shared::exception::CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   //--------------------------------------------------------------
   explicit CPythonException(const std::string& message)
      :CException(message)
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CPythonException() throw()
   {
   }
};

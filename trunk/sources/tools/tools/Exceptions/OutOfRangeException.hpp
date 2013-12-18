#pragma once

#include "Exception.hpp"

//--------------------------------------------------------------
/// \class Exception for value out of range
//--------------------------------------------------------------
class COutOfRangeException : public CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   //--------------------------------------------------------------
   COutOfRangeException(const std::string& message)
      :CException(std::string("Out of range " + message))
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~COutOfRangeException() throw()
   {
   }
};

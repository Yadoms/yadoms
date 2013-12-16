#pragma once

#include "Exception.h"

//--------------------------------------------------------------
/// \class Exception for non implemented part of code
//--------------------------------------------------------------
class CEmptyResultException : public CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   //--------------------------------------------------------------
   CEmptyResultException(const std::string & message)
      :CException(message.c_str())
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CEmptyResultException() throw()
   {
   }
};
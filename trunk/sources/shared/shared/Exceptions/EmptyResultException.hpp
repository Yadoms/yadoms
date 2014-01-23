#pragma once

#include "Exception.hpp"

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
      :CException(message)
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CEmptyResultException() throw()
   {
   }
};
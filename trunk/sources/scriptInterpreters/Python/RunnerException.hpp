#pragma once

#include <shared/exception/Exception.hpp>

//--------------------------------------------------------------
/// \class Exception for Python script runner
//--------------------------------------------------------------
class CRunnerException : public shared::exception::CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   //--------------------------------------------------------------
   CRunnerException(const std::string& message)
      :CException(message)
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CRunnerException() throw()
   {
   }
};

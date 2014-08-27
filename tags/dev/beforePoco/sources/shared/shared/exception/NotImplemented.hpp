#pragma once
#include "Exception.hpp"



namespace shared { namespace exception
{

   //--------------------------------------------------------------
   /// \class Exception for non implemented part of code
   //--------------------------------------------------------------
   class CNotImplemented : public CException
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CNotImplemented(const std::string& function)
         :CException(function + " is not yet implemented")
      {
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CNotImplemented() throw()
      {
      }
   };


} } // namespace shared::exception

// Helper macro
#define NOT_IMPLEMENTED    throw shared::exception::CNotImplemented(__FUNCTION__)



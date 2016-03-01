#pragma once
#include <shared/exception/Exception.hpp>

//--------------------------------------------------------------
/// \brief	Exception handling for protocol errors
//--------------------------------------------------------------
class COneWireException : public shared::exception::CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in]  message             Exception message
   //--------------------------------------------------------------
   explicit COneWireException(const std::string& message)
      :CException(message)
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~COneWireException() throw()
   {
   }
};

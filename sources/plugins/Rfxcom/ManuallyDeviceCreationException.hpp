#pragma once

#include <shared/exception/Exception.hpp>


//--------------------------------------------------------------
/// \brief	Exception handling for manually device creation
//--------------------------------------------------------------
class CManuallyDeviceCreationException : public shared::exception::CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in]  message             Exception message
   //--------------------------------------------------------------
   explicit CManuallyDeviceCreationException(const std::string& message)
      :CException(message)
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CManuallyDeviceCreationException() throw()
   {
   }
};


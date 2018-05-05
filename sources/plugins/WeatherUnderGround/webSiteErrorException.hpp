#pragma once
#include <shared/exception/Exception.hpp>

//--------------------------------------------------------------
/// \brief	Exception handling for protocol errors
//--------------------------------------------------------------
class CWebSiteErrorException : public shared::exception::CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in]  error               Exception message
   //--------------------------------------------------------------
   explicit CWebSiteErrorException(const std::string& error)
      : CException(error)
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CWebSiteErrorException() throw()
   {
   }
};


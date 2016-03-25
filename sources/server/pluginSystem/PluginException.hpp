#pragma once

#include <shared/exception/Exception.hpp>


//--------------------------------------------------------------
/// \brief	Exception handling for plugin
//--------------------------------------------------------------
class CPluginException : public shared::exception::CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in]  message             Exception message
   //--------------------------------------------------------------
   explicit CPluginException(const std::string& message)
      :CException(message)
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CPluginException() throw()
   {
   }
};


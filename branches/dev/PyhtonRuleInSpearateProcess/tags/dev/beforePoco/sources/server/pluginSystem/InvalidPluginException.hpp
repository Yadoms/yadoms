//
// InvalidPluginException.h
//
// Invalid plugin exception
//
#pragma once

#include <shared/exception/Exception.hpp>

namespace pluginSystem
{

   //--------------------------------------------------------------
   /// \class Configuration loading error exception
   //--------------------------------------------------------------
   class CInvalidPluginException : public shared::exception::CException
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in]  plugin              Plugin name
      //--------------------------------------------------------------
      CInvalidPluginException(const std::string& plugin)
         :CException(std::string("Invalid plugin found " + plugin).c_str())
      {
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CInvalidPluginException() throw()
      {
      }
   };

} // namespace pluginSystem

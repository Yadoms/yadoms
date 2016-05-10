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
      explicit CInvalidPluginException(const std::string& plugin)
         :CException(std::string("Invalid plugin found ") + plugin)
      {
      }

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in]  plugin              Plugin name
      /// \param[in]  message             Message
      //--------------------------------------------------------------
      CInvalidPluginException(const std::string& plugin, const std::string& message)
         :CException((boost::format("Invalid plugin found %1% : %2%") % plugin % message).str())
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

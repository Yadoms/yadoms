//
// Factory.h
//
// yadoms-plugin factory
//
#pragma once


#include <shared/plugin/IPlugin.h>
#include <shared/plugin/information/IInformation.h>
#include "InvalidPluginException.hpp"

namespace pluginSystem
{

   class IFactory 
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IFactory()
      {
      }

      //--------------------------------------------------------------
      /// \brief	    Construct a plugin instance (call the construct library method)
      /// \return     a new plugin instance
      //-------------------------------------------------------------
      virtual shared::plugin::IPlugin* construct() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the plugin library path
      /// \return     the plugin library path
      //-------------------------------------------------------------
      virtual const boost::filesystem::path& getLibraryPath() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get information about this plugin
      /// \return     information about this plugin
      //-------------------------------------------------------------
      virtual boost::shared_ptr<const shared::plugin::information::IInformation> getInformation() const = 0;
   };



} // namespace pluginSystem

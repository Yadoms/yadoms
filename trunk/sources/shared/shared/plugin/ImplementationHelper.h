#pragma once

#include "IPlugin.h"
#include "information/Information.h"
#include <shared/Export.h>
#include <shared/FileSystemExtension.h>
#include <boost/shared_ptr.hpp>

//--------------------------------------------------------------
/// \brief		                  Plugin implementation helper
/// \param pluginClassName       The name of the main plugin class (implementing shared::plugin::IPlugin)
/// \note                        This macro create and export needed functions for the plugin
//--------------------------------------------------------------
#define IMPLEMENT_PLUGIN(pluginClassName)                                                                                  \
   EXPORT_LIBRARY_FUNCTION shared::plugin::IPlugin* construct()                                                            \
   {                                                                                                                       \
      return new pluginClassName();                                                                                        \
   }                                                                                                                       \
                                                                                                                           \
   static boost::shared_ptr<const shared::plugin::information::CInformation> Informations;                                 \
   EXPORT_LIBRARY_FUNCTION const shared::plugin::information::IInformation& getInformation()                               \
   {                                                                                                                       \
      if (!Informations)                                                                                                   \
         Informations.reset(new shared::plugin::information::CInformation(shared::CFileSystemExtension::getModulePath())); \
      return *Informations;                                                                                                \
   }

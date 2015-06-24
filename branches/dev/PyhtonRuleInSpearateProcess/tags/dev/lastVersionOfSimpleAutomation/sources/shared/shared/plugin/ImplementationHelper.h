#pragma once

#include <shared/Export.h>
#include "IPlugin.h"


//--------------------------------------------------------------
/// \brief		                  Plugin implementation helper
/// \param pluginClassName       The name of the main plugin class (implementing shared::plugin::IPlugin)
/// \note                        This macro create and export needed functions for the plugin
//--------------------------------------------------------------
#define IMPLEMENT_PLUGIN(pluginClassName)                                                                                  \
   EXPORT_LIBRARY_FUNCTION shared::plugin::IPlugin* construct()                                                            \
   {                                                                                                                       \
      return new pluginClassName();                                                                                        \
   }

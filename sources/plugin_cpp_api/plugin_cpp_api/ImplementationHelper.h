#pragma once

#include "IPlugin.h"
#include "PluginContext.h"


//--------------------------------------------------------------
/// \brief		                  Plugin implementation helper
/// \param pluginClassName       The name of the main plugin class (implementing shared::plugin::IPlugin)
/// \note                        This macro create main function that initialize plugin
//--------------------------------------------------------------
//TODO à revoir
//TODO virer paramètre pluginClassName ?
#define IMPLEMENT_PLUGIN(pluginClassName)                                                                                  \
   int main(int argc, char **argv) \
{\
   auto pluginContext(argc, argv); \
   pluginClassName().doWork(pluginContext.context()); \
   return 0;\
}

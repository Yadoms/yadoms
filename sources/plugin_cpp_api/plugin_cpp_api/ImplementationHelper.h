#pragma once

#include "PluginMain.h"

//--------------------------------------------------------------
/// \brief		                  Plugin implementation helper
/// \param pluginClassName       The name of the main plugin class (implementing shared::plugin::IPlugin)
/// \note                        This macro create main function that initialize plugin
//--------------------------------------------------------------
#define IMPLEMENT_PLUGIN(pluginClassName)                            \
   int main(int argc, char **argv)                                   \
   {                                                                 \
      doMain(argc, argv, boost::make_shared<pluginClassName>());     \
   }

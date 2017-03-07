#pragma once
#include "IPlugin.h"

extern int doMain(int argc, char **argv, boost::shared_ptr<plugin_cpp_api::IPlugin> plugin);

//--------------------------------------------------------------
/// \brief		                  Plugin implementation helper
/// \param pluginClassName       The name of the main plugin class (implementing shared::plugin::IPlugin)
/// \note                        This macro creates main function that initialize plugin
//--------------------------------------------------------------
#define IMPLEMENT_PLUGIN(pluginClassName)                               \
   int main(int argc, char **argv)                                      \
   {                                                                    \
      return doMain(argc, argv, boost::make_shared<pluginClassName>()); \
   }

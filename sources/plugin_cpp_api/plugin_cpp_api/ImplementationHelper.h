#pragma once

#include "IPluginContext.h"

//--------------------------------------------------------------
/// \brief		                  Plugin implementation helper
/// \param pluginClassName       The name of the main plugin class (implementing shared::plugin::IPlugin)
/// \note                        This macro create main function that initialize plugin
//--------------------------------------------------------------
//TODO à revoir
#define IMPLEMENT_PLUGIN(pluginClassName)                                                                                  \
   int main(int argc, char **argv) \
   { \
      auto pluginContext = CreatePluginContext(argc, argv, boost::make_shared<pluginClassName>()); \
      pluginContext->run(); \
      return static_cast<int>(pluginContext->getReturnCode());\
   }

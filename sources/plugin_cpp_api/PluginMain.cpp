#include "stdafx.h"
#include "PluginContext.h"
#include "plugin_cpp_api/IPlugin.h"
#include <shared/process/ApplicationStopHandler.h>


int doMain(int argc,
           char** argv,
           boost::shared_ptr<plugin_cpp_api::IPlugin> plugin)
{
   try
   {
      auto stopHandler = boost::make_shared<shared::process::CApplicationStopHandler>(false);
      // Termination should be asked by Yadoms with IPC
      stopHandler->setApplicationStopHandler(boost::function<bool()>());

      auto pluginContext = boost::make_shared<plugin_cpp_api::CPluginContext>(argc, argv, plugin);
      pluginContext->run();
      return pluginContext->getReturnCode();
   }
   catch (std::invalid_argument& e)
   {
      std::cerr << "Unable to start plugin : " << e.what() << std::endl;
      return plugin_cpp_api::IPluginContext::kStartError;
   }
   catch (...)
   {
      std::cerr << "Plugin crashed" << std::endl;
      return plugin_cpp_api::IPluginContext::kRuntimeError;
   }
}


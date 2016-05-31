#include "stdafx.h"
#include "PluginContext.h"
#include "plugin_cpp_api/IPlugin.h"
#include <shared/process/ApplicationStopHandler.h>

//TODO      Ajouter ce lien dans le wiki plugin/how to debug my plugin : https://msdn.microsoft.com/en-us/library/a329t4ed(v=vs.90).aspx

int doMain(int argc, char **argv, boost::shared_ptr<plugin_cpp_api::IPlugin> plugin)
{
   try
   {
      shared::event::CEventHandler stopEvenHandler;
      enum { kPluginStopped = shared::event::kUserFirstId };
      auto stopHandler = boost::make_shared<shared::process::CApplicationStopHandler>(false);
      stopHandler->setApplicationStopHandler([&]() -> bool
      {
         // Termination should be asked by Yadoms with IPC, so just wait for end
         return stopEvenHandler.waitForEvents(boost::posix_time::seconds(30)) == kPluginStopped;
      });

      auto pluginContext = boost::make_shared<plugin_cpp_api::CPluginContext>(argc, argv, plugin);
      pluginContext->run();
      stopEvenHandler.postEvent(kPluginStopped);
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

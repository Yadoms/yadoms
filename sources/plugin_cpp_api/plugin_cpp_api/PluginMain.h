#pragma once
#include "IPlugin.h"

namespace plugin_cpp_api
{
   enum EReturnCode
   {
      kOk = 0,
      kStartError,         // Plugin failed to start
      kRuntimeError,       // Plugin crashed at runtime
      kUnexpectedStop      // Plugin has stopped without stop requested
   };


   int doMain(int argc, char** argv, boost::shared_ptr<IPlugin> plugin);
} // namespace plugin_cpp_api

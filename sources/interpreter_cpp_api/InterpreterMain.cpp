#include "stdafx.h"
#include "InterpreterContext.h"
#include "interpreter_cpp_api/IInterpreter.h"
#include <shared/process/ApplicationStopHandler.h>


int doMain(int argc,
           char** argv,
           boost::shared_ptr<interpreter_cpp_api::IInterpreter> interpreter)
{
   try
   {
      shared::event::CEventHandler stopEvenHandler;
      enum
         {
            kPluginStopped = shared::event::kUserFirstId
         };
      auto stopHandler = boost::make_shared<shared::process::CApplicationStopHandler>(false);
      stopHandler->setApplicationStopHandler([&]() -> bool
         {
            // Termination should be asked by Yadoms with IPC, so just wait for end
            return stopEvenHandler.waitForEvents(boost::posix_time::seconds(30)) == kPluginStopped;
         });

      auto pluginContext = boost::make_shared<interpreter_cpp_api::CInterpreterContext>(argc, argv, interpreter);
      pluginContext->run();
      stopEvenHandler.postEvent(kPluginStopped);
      return pluginContext->getReturnCode();
   }
   catch (std::invalid_argument& e)
   {
      std::cerr << "Unable to start interpreter : " << e.what() << std::endl;
      return interpreter_cpp_api::IInterpreterContext::kStartError;
   }
   catch (...)
   {
      std::cerr << "Interpreter crashed" << std::endl;
      return interpreter_cpp_api::IInterpreterContext::kRuntimeError;
   }
}


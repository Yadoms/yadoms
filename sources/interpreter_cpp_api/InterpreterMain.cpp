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
      auto stopHandler = boost::make_shared<shared::process::CApplicationStopHandler>(false);
      // Termination should be asked by Yadoms with IPC
      stopHandler->setApplicationStopHandler(boost::function<bool()>());

      auto interpreterContext = boost::make_shared<interpreter_cpp_api::CInterpreterContext>(argc, argv, interpreter);
      interpreterContext->run();
      return interpreterContext->getReturnCode();
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


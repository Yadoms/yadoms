#pragma once
#include "IInterpreter.h"

extern int doMain(int argc, char **argv, boost::shared_ptr<interpreter_cpp_api::IPlugin> plugin);

//--------------------------------------------------------------
/// \brief		                  Interpreter implementation helper
/// \param interpreterClassName  The name of the main interpreter class (implementing shared::interpreter::IInterpreter)
/// \note                        This macro creates main function that initialize interpreter
//--------------------------------------------------------------
#define IMPLEMENT_INTERPRETER(interpreterClassName)                           \
   int main(int argc, char **argv)                                            \
   {                                                                          \
      return doMain(argc, argv, boost::make_shared<interpreterClassName>());  \
   }

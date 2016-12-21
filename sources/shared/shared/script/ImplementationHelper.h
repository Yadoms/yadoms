#pragma once

#include <shared/DynamicLibraryExport.h>
#include "IInterpreter.h"


//--------------------------------------------------------------
/// \brief		                        Script interpreter implementation helper
/// \param scriptInterpreterClassName  The name of the main script interpreter class (implementing shared::script::IInterpreter)
/// \note                              This macro creates and exports needed functions for the script interpreter
//--------------------------------------------------------------
#define IMPLEMENT_SCRIPT_INTERPRETER(scriptInterpreterClassName)                                                           \
   EXPORT_LIBRARY_FUNCTION shared::script::IInterpreter* construct(const boost::filesystem::path& interpreterPath)         \
   {                                                                                                                       \
      return new scriptInterpreterClassName(interpreterPath);                                                              \
   }

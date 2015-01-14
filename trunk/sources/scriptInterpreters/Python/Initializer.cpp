#include "stdafx.h"
#include "Initializer.h"
#include "PythonLibInclude.h"
#include <shared/FileSystemExtension.h>

CInitializer::CInitializer()
{
   // Python API documentation says that Py_SetProgramName doesn't modify its parameter, but this parameter
   // is not defined as const...
   Py_SetProgramName(const_cast<char*>(shared::CFileSystemExtension::getModuleFullPath().string().c_str()));
   Py_InitializeEx(0);
}

CInitializer::~CInitializer()
{
   Py_Finalize();
}

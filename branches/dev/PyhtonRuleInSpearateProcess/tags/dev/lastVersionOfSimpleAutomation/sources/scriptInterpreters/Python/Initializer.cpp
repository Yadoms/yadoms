#include "stdafx.h"
#include "Initializer.h"
#include <shared/FileSystemExtension.h>

CInitializer::CInitializer()
   :m_globalPythonThreadState(NULL)
{
   // Python API documentation says that Py_SetProgramName doesn't modify its parameter, but this parameter
   // is not defined as const...
   Py_SetProgramName(const_cast<char*>(shared::CFileSystemExtension::getModuleFullPath().string().c_str()));
   Py_InitializeEx(0);
   PyEval_InitThreads();
   m_globalPythonThreadState = PyEval_SaveThread();
}

CInitializer::~CInitializer()
{
   PyEval_RestoreThread(m_globalPythonThreadState);
   Py_Finalize();
}

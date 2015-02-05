#include "stdafx.h"
#include "ScriptLoader.h"
#include "PythonLibInclude.h"
#include "RunnerException.hpp"
#include "PythonBorrowedObject.h"
#include "ScriptFile.h"


CScriptLoader::CScriptLoader(const std::string& scriptName)
   :m_scriptFile(new CScriptFile(scriptName))
{
}

CScriptLoader::~CScriptLoader()
{
}

void CScriptLoader::load()
{
   // Check script path
   if (!m_scriptFile->exists())
      throw CRunnerException((boost::format("script file %1% doesn't exist") % m_scriptFile->pathName()).str());

   // Add script path to Python system path
   CPythonBorrowedObject sysPathObject(PySys_GetObject(static_cast<char*>("path")));
   if (sysPathObject.isNull())
      throw CRunnerException((boost::format("unable to get sys.path object for %1%") % m_scriptFile->pathName()).str());
   CPythonObject scriptAbsolutePath(PyString_FromString(m_scriptFile->abslouteParentPath().string().c_str()));
   PyList_Append(*sysPathObject, *scriptAbsolutePath);

   // Convert script path as Python string
   CPythonObject pyModuleName(PyString_FromString(m_scriptFile->module().c_str()));
   if (pyModuleName.isNull())
      throw CRunnerException((boost::format("unable to import module %1%") % m_scriptFile->module()).str());

   // Import script to Python interpreter
   m_pyModule.reset(PyImport_Import(*pyModuleName));
   if (m_pyModule.isNull())
      throw CRunnerException("unable to create Python module");

   // Get the entry point function pointer
   m_pyMainFunction.reset(PyObject_GetAttrString(*m_pyModule, m_scriptFile->entryPoint().c_str()));
   if (m_pyMainFunction.isNull() || PyCallable_Check(*m_pyMainFunction) == 0)
      throw CRunnerException((boost::format("%1% function was not found") % m_scriptFile->entryPoint()).str());
}

CPythonObject& CScriptLoader::module()
{
   return m_pyModule;
}        

CPythonObject& CScriptLoader::yMain()
{
   return m_pyMainFunction;
}
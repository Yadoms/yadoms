#include "stdafx.h"
#include "ScriptLoader.h"
#include "PythonLibInclude.h"
#include "RunnerException.hpp"
#include "PythonBorrowedObject.h"


// Function name of the Python script entry point
static const std::string ScriptEntryPoint("yMain");

CScriptLoader::CScriptLoader(const std::string& scriptPath)
   :m_scriptPath(scriptPath)
{
}

CScriptLoader::~CScriptLoader()
{
}

void CScriptLoader::load()
{
   //TODO  et '/' et '\' dans les chemins
   // Check script path
   if (!boost::filesystem::exists(m_scriptPath))
      throw CRunnerException((boost::format("script file %1% doesn't exist") % m_scriptPath).str());

   // Add script path to Python system path
   CPythonBorrowedObject sysPathObject(PySys_GetObject("path"));
   CPythonObject scriptAbsolutePath(PyString_FromString(boost::filesystem::absolute(m_scriptPath).parent_path().string().c_str()));
   PyList_Append(*sysPathObject, *scriptAbsolutePath);

   // Convert script path as Python string
   std::string moduleName(boost::filesystem::path(m_scriptPath).stem().string());
   CPythonObject pyModuleName(PyString_FromString(moduleName.c_str()));
   if (pyModuleName.isNull())
      throw CRunnerException((boost::format("unable to import module %1%") % moduleName).str());

   // Import script to Python interpreter
   m_pyModule.reset(PyImport_Import(*pyModuleName));
   if (m_pyModule.isNull())
      throw CRunnerException("unable to create Python module");

   // Get the entry point function pointer
   m_pyMainFunction.reset(PyObject_GetAttrString(*m_pyModule, ScriptEntryPoint.c_str()));
   if (m_pyMainFunction.isNull() || PyCallable_Check(*m_pyMainFunction) == 0)
      throw CRunnerException((boost::format("%1% function was not found") % ScriptEntryPoint).str());
}

CPythonObject& CScriptLoader::yMain()
{
   return m_pyMainFunction;
}
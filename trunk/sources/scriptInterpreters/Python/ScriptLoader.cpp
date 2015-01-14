#include "stdafx.h"
#include "ScriptLoader.h"
#include "PythonLibInclude.h"
#include "RunnerException.hpp"
#include "PythonBorrowedObject.h"


// Main scripts folder (subfolder of Yadoms main folder)
static const std::string ScriptsFolder("scripts");

// Add prefix to filename, to avoid confusing with existing Python module
// (In Python, filename also serves as module name, and modules are globals)
// So script full path becomes : {scriptName}/yadoms_{scriptName}
static const std::string ScriptFilenamePrefix("yadoms_");

// Function name of the Python script entry point
static const std::string ScriptEntryPoint("yMain");

CScriptLoader::CScriptLoader(const std::string& scriptName)
   :m_scriptFile(boost::filesystem::path(ScriptsFolder) / boost::filesystem::path(scriptName) / boost::filesystem::path(ScriptFilenamePrefix + scriptName))
{
}

CScriptLoader::~CScriptLoader()
{
}

void CScriptLoader::load()
{
   // Check script path
   if (!boost::filesystem::exists(m_scriptFile))
      throw CRunnerException((boost::format("script file %1% doesn't exist") % m_scriptFile).str());

   // Add script path to Python system path
   CPythonBorrowedObject sysPathObject(PySys_GetObject("path"));
   CPythonObject scriptAbsolutePath(PyString_FromString(boost::filesystem::absolute(m_scriptFile).parent_path().string().c_str()));
   PyList_Append(*sysPathObject, *scriptAbsolutePath);

   // Convert script path as Python string
   std::string moduleName(m_scriptFile.stem().string());
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
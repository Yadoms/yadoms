#include "stdafx.h"
#include "Python.h"
#include "Initializer.h"
#include <shared/script/ImplementationHelper.h>
#include "PythonLibInclude.h"
#include <shared/Log.h>

// Declare the script interpreter
IMPLEMENT_SCRIPT_INTERPRETER(CPython)


CPython::CPython()
   :m_initializer(new CInitializer)
{
}

CPython::~CPython()
{
}

bool CPython::isAvailable() const
{
   // As Python is staticaly linked, Python library is already loaded.
   // Just test it asking its version (and check version number).
   const std::string pythonVersion(Py_GetVersion());
   if (pythonVersion.empty())
      return false;

   static const std::string expectedVersion("2.7");
   size_t versionFoundPosition = pythonVersion.find(expectedVersion);
   if (versionFoundPosition == std::string::npos || versionFoundPosition != 0)
      return false;

   return true;
}

bool CPython::canInterpret(const std::string& scriptPath) const
{
   //TODO : structure en répertoire + fichier (pour permettre d'avoir des ressources par script) ?
   if (!boost::filesystem::exists(scriptPath))
   {
      YADOMS_LOG(error) << scriptPath << " : can not interpret, file doesn't exist";
      return false;
   }

   // TODO faire un CPythonObject
   PyObject* pyScriptPath = PyString_FromString(scriptPath.c_str());
   PyObject* pyModule = PyImport_Import(pyScriptPath);
   Py_DECREF(pyScriptPath);

   if (pyModule == NULL)
   {
      YADOMS_LOG(error) << scriptPath << " : can not interpret, unable to create Python module";
      return false;
   }

   //TODO : charger le fichier et vérifier que la fonction "yMain" existe
   return false;
}

boost::shared_ptr<shared::script::IRunner> CPython::createRunner(const std::string& scriptPath, const shared::CDataContainer& scriptConfiguration) const
{
   //TODO
   return boost::shared_ptr<shared::script::IRunner>();
}
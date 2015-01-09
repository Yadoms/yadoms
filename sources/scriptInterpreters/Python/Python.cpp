#include "stdafx.h"
#include "Python.h"
#if defined WIN32 && defined _DEBUG
#undef _DEBUG // Undef _DEBUG to use only release version of Python.lib. It permits to download the Pyhton installer version (https://www.python.org/downloads/windows/)
#include <Python.h>
#define _DEBUG
#endif
#include <shared/script/ImplementationHelper.h>

// Declare the script interpreter
IMPLEMENT_SCRIPT_INTERPRETER(CPython)


CPython::CPython()
{
}

CPython::~CPython()
{
}

bool CPython::isAvailable() const
{
   //TODO : recherche et test de Python
   return true;
}

bool CPython::canInterpret(const std::string& scriptPath) const
{
   //TODO : charger le fichier et vérifier que la fonction "yMain" existe
   return true;
}

boost::shared_ptr<shared::script::IScriptRunner> CPython::createRunner(const std::string& scriptPath, const shared::CDataContainer& scriptConfiguration) const
{
   //TODO
   return boost::shared_ptr<shared::script::IScriptRunner>();
}
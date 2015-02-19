#include "stdafx.h"
#include "ScriptFile.h"
#include <shared/exception/InvalidParameter.hpp>


// Be careful, in Python, filename is also the Python module name, and modules are globals.
static const std::string ScriptName("yadomsScript");
static const std::string Module(ScriptName);

// Standard Pyhton scripts extension
static const std::string PythonExtension("py");

// Function name of the Python script entry point
static const std::string ScriptEntryPoint("yMain");

CScriptFile::CScriptFile(const std::string& scriptPath)
   :m_scriptFile(boost::filesystem::path(scriptPath) / boost::filesystem::path(ScriptName).replace_extension(PythonExtension))
{
}

CScriptFile::~CScriptFile()
{
}

const boost::filesystem::path& CScriptFile::pathName() const
{
   return m_scriptFile;
}

const boost::filesystem::path CScriptFile::abslouteParentPath() const
{
   return boost::filesystem::absolute(m_scriptFile).parent_path();
}  

const std::string& CScriptFile::module() const
{
   return Module;
}

const std::string& CScriptFile::entryPoint() const
{
   return ScriptEntryPoint;
}

bool CScriptFile::exists() const
{
   return boost::filesystem::exists(m_scriptFile);
}

std::string CScriptFile::read() const
{
   if (!exists())
      throw shared::exception::CInvalidParameter(m_scriptFile.string());

   std::ifstream file(m_scriptFile.string().c_str());
   if (!file.is_open())
      throw shared::exception::CInvalidParameter(m_scriptFile.string());

   std::istreambuf_iterator<char> eos;
   return std::string(std::istreambuf_iterator<char>(file), eos);
}

void CScriptFile::write(const std::string& content) const
{
   std::ofstream file(m_scriptFile.string().c_str());
   file << content;
}

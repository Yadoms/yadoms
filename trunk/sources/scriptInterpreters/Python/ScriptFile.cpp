#include "stdafx.h"
#include "ScriptFile.h"
#include <shared/exception/InvalidParameter.hpp>


const std::string CScriptFile::ScriptName("yadomsScript");
const std::string CScriptFile::Module(ScriptName);
const std::string CScriptFile::PythonExtension("py");
const std::string CScriptFile::ScriptEntryPoint("yMain");

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

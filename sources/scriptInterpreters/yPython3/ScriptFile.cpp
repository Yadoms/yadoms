#include "stdafx.h"
#include "ScriptFile.h"
#include <shared/exception/InvalidParameter.hpp>


const std::string CScriptFile::ScriptName("yadomsScript");
const std::string CScriptFile::Module(ScriptName);
const std::string CScriptFile::PythonExtension("py");
const std::string CScriptFile::ScriptEntryPoint("yMain");

CScriptFile::CScriptFile(const boost::filesystem::path& scriptPath)
   :m_scriptFile(scriptPath / boost::filesystem::path(ScriptName).replace_extension(PythonExtension))
{
}

CScriptFile::~CScriptFile()
{
}

const boost::filesystem::path& CScriptFile::pathName() const
{
   return m_scriptFile;
}

boost::filesystem::path CScriptFile::abslouteParentPath() const
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

std::string CScriptFile::pythonFileRead(const std::string& filename)
{
   if (!boost::filesystem::exists(filename))
      throw shared::exception::CInvalidParameter(filename);

   std::ifstream file(filename.c_str());
   if (!file.is_open())
      throw shared::exception::CInvalidParameter(filename);

   const std::istreambuf_iterator<char> eos;
   std::string content(std::istreambuf_iterator<char>(file), eos);
   return content.erase(0, 3); // Remove UTF-8 BOM
}

std::string CScriptFile::read() const
{
   return pythonFileRead(m_scriptFile.string());
}

void CScriptFile::write(const std::string& content) const
{
   std::ofstream file(m_scriptFile.string().c_str(), std::ios::out | std::ios::binary);
   file << "\xef\xbb\xbf";    // UTF-8 BOM
   file << content;
}

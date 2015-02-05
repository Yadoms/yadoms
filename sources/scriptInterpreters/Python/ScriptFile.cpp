#include "stdafx.h"
#include "ScriptFile.h"


// Main scripts folder (subfolder of Yadoms main folder)
static const std::string ScriptsFolder("scripts");

// Add prefix to filename, to avoid confusing with existing Python module
// (In Python, filename also serves as module name, and modules are globals)
// So script full path becomes : {scriptName}/yadoms_{scriptName}
static const std::string ScriptFilenamePrefix("yadoms_");

// Standard Pyhton scripts extension
static const std::string PythonExtension("py");

// Function name of the Python script entry point
static const std::string ScriptEntryPoint("yMain");

CScriptFile::CScriptFile(const std::string& scriptName)
   :m_scriptFile(boost::filesystem::path(ScriptsFolder) / boost::filesystem::path(scriptName) / boost::filesystem::path(ScriptFilenamePrefix + scriptName).replace_extension(PythonExtension)),
   m_module(ScriptFilenamePrefix + scriptName)
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
   return m_module;
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
      return fromTemplate();

   std::ifstream file(m_scriptFile.string().c_str());
   if (!file.is_open())
      return fromTemplate();

   std::istreambuf_iterator<char> eos;
   return std::string(std::istreambuf_iterator<char>(file), eos);
}

void CScriptFile::write(const std::string& content) const
{
   std::ofstream file(m_scriptFile.string().c_str());
   file << content;
}

std::string CScriptFile::fromTemplate() const
{
   static const std::string scriptTemplate(                                      \
      "\n"                                                                       \
      "# " + ScriptEntryPoint + " is the main entry point of your script"        \
      "def " + ScriptEntryPoint + "():\n"                                        \
      "   # ... add your code here ..."                                          \
      "\n"                                                                       \
      );

   return scriptTemplate;
}

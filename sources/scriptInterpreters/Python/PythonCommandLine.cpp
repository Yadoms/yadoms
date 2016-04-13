#include "stdafx.h"
#include "PythonCommandLine.h"
#include <shared/Executable.h>


CPythonCommandLine::CPythonCommandLine(const boost::filesystem::path& workingDirectory,
                                                            const std::string& pluginName,
                                                            const std::vector<std::string> parameters)
   : m_workingDirectory(workingDirectory), m_pluginName(pluginName), m_args(parameters)
{
}

CPythonCommandLine::~CPythonCommandLine()
{
}

std::string CPythonCommandLine::executable() const
{
   return shared::CExecutable::ToFileName(m_pluginName);
}

const boost::filesystem::path& CPythonCommandLine::workingDirectory() const
{
   return m_workingDirectory;
}

const std::vector<std::string>& CPythonCommandLine::args() const
{
   return m_args;
}


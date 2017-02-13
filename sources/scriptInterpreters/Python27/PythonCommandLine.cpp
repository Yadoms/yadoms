#include "stdafx.h"
#include "PythonCommandLine.h"


CPythonCommandLine::CPythonCommandLine(const boost::filesystem::path& executablePath,
                                       const boost::filesystem::path& workingDirectory,
                                       const std::vector<std::string> parameters)
   : m_executable(executablePath),
     m_workingDirectory(workingDirectory),
     m_args(parameters)
{
}

CPythonCommandLine::~CPythonCommandLine()
{
}

const boost::filesystem::path& CPythonCommandLine::executable() const
{
   return m_executable;
}

const boost::filesystem::path& CPythonCommandLine::workingDirectory() const
{
   return m_workingDirectory;
}

const std::vector<std::string>& CPythonCommandLine::args() const
{
   return m_args;
}


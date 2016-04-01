#include "stdafx.h"
#include "NativeExecutableCommandLine.h"

namespace pluginSystem
{
   CNativeExecutableCommandLine::CNativeExecutableCommandLine(const boost::filesystem::path& workingDirectory,
                                                              const std::string& executableName,
                                                              const std::vector<std::string> parameters)
      :m_workingDirectory(workingDirectory), m_executableName(executableName), m_args(parameters)
   {
   }

   CNativeExecutableCommandLine::~CNativeExecutableCommandLine()
   {
   }

   std::string CNativeExecutableCommandLine::executable() const
   {
      return m_executableName;
   }

   const boost::filesystem::path& CNativeExecutableCommandLine::workingDirectory() const
   {
      return m_workingDirectory;
   }

   const std::vector<std::string>& CNativeExecutableCommandLine::args() const
   {
      return m_args;
   }

   const std::string& CNativeExecutableCommandLine::toString() const
   {
      return std::string();//TODO (si utile)
   }

} // namespace pluginSystem

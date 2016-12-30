#include "stdafx.h"
#include "NativeExecutableCommandLine.h"

namespace shared
{
   namespace process
   {
      CNativeExecutableCommandLine::CNativeExecutableCommandLine(const boost::filesystem::path& executablePath,
                                                                 const boost::filesystem::path& workingDirectory,
                                                                 const std::vector<std::string>& parameters)
         : m_executablePath(executablePath),
           m_workingDirectory(workingDirectory),
           m_args(parameters)
      {
      }

      CNativeExecutableCommandLine::~CNativeExecutableCommandLine()
      {
      }

      const boost::filesystem::path& CNativeExecutableCommandLine::executable() const
      {
         return m_executablePath;
      }

      const boost::filesystem::path& CNativeExecutableCommandLine::workingDirectory() const
      {
         return m_workingDirectory;
      }

      const std::vector<std::string>& CNativeExecutableCommandLine::args() const
      {
         return m_args;
      }
   }
} // namespace shared::process



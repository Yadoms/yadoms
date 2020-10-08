#include "stdafx.h"
#include "PythonExecutable.h"
#include "PythonExecutablePath.h"
#include <shared/Executable.h>
#include <shared/Log.h>
#include <shared/versioning/SemVer.h>
#include <regex>
#include <algorithm>

CPythonExecutable::CPythonExecutable(const std::string& pythonForcedPath)
   : m_inSystemPath(false),
     m_found(findPythonDirectory(pythonForcedPath, m_executableDirectory, m_inSystemPath)),
     m_version(readPythonVersion(m_executableDirectory))
{
}

bool CPythonExecutable::found() const
{
   return m_found;
}

std::string CPythonExecutable::version() const
{
   return m_version;
}

bool CPythonExecutable::inSystemPath() const
{
   return m_inSystemPath;
}

boost::filesystem::path CPythonExecutable::path() const
{
   return m_executableDirectory / filename();
}

std::string CPythonExecutable::filename() const
{
   return shared::CExecutable::ToFileName(CPythonExecutablePath::getExecutableName());
}

bool CPythonExecutable::findPythonDirectory(const std::string& pythonForcedPath,
                                            boost::filesystem::path& pythonDirectory,
                                            bool& inSystemPath)
{
   // First search for path configured in configuration file
   if (!pythonForcedPath.empty())
   {
      // Python path is forced (defined in yadoms.ini), use it in priority
      if (isPythonIn(pythonForcedPath, pythonDirectory, inSystemPath))
         return true;
   }

   // Now try to auto-detect path
   // Retrieve common paths for the system
   const auto commonPaths = CPythonExecutablePath::getCommonPythonPaths();

   if (std::any_of(commonPaths->begin(),
                   commonPaths->end(),
                   [&pythonDirectory, &inSystemPath](const auto& path)
                   {
                      return isPythonIn(path, pythonDirectory, inSystemPath);
                   }))
      return true;

   if (isPythonIn(boost::filesystem::path(), pythonDirectory, inSystemPath)) // Empty directory to search in system path
      return true;

   YADOMS_LOG(error) << "Python 3 executable not found";
   return false;
}

bool CPythonExecutable::isPythonIn(const boost::filesystem::path& directory,
                                   boost::filesystem::path& pythonDirectory,
                                   bool& inSystemPath)
{
   const auto version = readPythonVersion(directory);

   if (!isValidPythonVersion(version))
   {
      YADOMS_LOG(information) << "Python 3 executable not found in " << (directory.empty()
                                                                            ? "the system path"
                                                                            : directory.string());
      return false;
   }
   YADOMS_LOG(information) << "Python 3 executable found in " << (directory.empty()
                                                                     ? "the system path"
                                                                     : directory.string());
   pythonDirectory = directory;
   inSystemPath = directory.empty();
   return true;
}

std::string CPythonExecutable::readPythonVersion(const boost::filesystem::path& initialDirectory)
{
   try
   {
      const auto& command = initialDirectory / shared::CExecutable::ToFileName(
         CPythonExecutablePath::getExecutableName());

      Poco::Process::Args args;
      args.push_back("--version");

      Poco::Pipe outPipe;
      const auto processHandle = Poco::Process::launch(command.string(), args, nullptr, &outPipe, &outPipe);
      processHandle.wait();

      Poco::PipeInputStream iStr(outPipe);
      std::ostringstream oStr;
      Poco::StreamCopier::copyStream(iStr, oStr);
      return oStr.str();
   }
   catch (Poco::Exception& ex)
   {
      YADOMS_LOG(information) << "Unable to read Python 3 version, " << ex.what();
      return std::string();
   }
}

bool CPythonExecutable::isValidPythonVersion(const std::string& pythonVersion)
{
   std::smatch matches;
   if (!std::regex_search(pythonVersion, matches, std::regex(R"(Python\s*(3\.\d+.\d+))")) ||
      matches.size() != 2)
      return false;

   try
   {
      static const shared::versioning::CSemVer MinimalExpectedVersion("3.6.0");
      const shared::versioning::CSemVer readVersion(matches[1]);

      return readVersion >= MinimalExpectedVersion;
   }
   catch (const std::exception&)
   {
      YADOMS_LOG(information) << "Invalid Python version " << matches[1];
      return false;
   }
}

#include "stdafx.h"
#include "PythonExecutablePath.h"
#include <Poco/Util/WinRegistryKey.h>


void CPythonExecutablePath::getCommonPythonPaths(std::vector<boost::filesystem::path>& paths)
{
   Poco::Util::WinRegistryKey check32(HKEY_LOCAL_MACHINE, "SOFTWARE\\Python\\PyLauncher", true);
   if (check32.exists())
   {
      const auto pythonPath = check32.getString("InstallDir");
      if (!pythonPath.empty())
         paths.push_back(boost::filesystem::path(pythonPath));
   }

   Poco::Util::WinRegistryKey check64(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Python\\PyLauncher", true);
   if (check64.exists())
   {
      const auto pythonPath = check64.getString("InstallDir");
      if (!pythonPath.empty())
         paths.push_back(boost::filesystem::path(pythonPath));
   }

   // Remove duplicates
   std::sort(paths.begin(), paths.end());
   paths.erase(std::unique(paths.begin(), paths.end()), paths.end());
}

const std::string& CPythonExecutablePath::getExecutableName()
{
   static const std::string ExecutableName("py.exe");
   return ExecutableName;
}

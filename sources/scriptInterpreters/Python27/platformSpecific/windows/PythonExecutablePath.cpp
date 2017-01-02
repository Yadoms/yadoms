#include "stdafx.h"
#include "PythonExecutablePath.h"
#include <Poco/Util/WinRegistryKey.h>
#include <Poco/Environment.h>
#include <Poco/Exception.h>

void CPythonExecutablePath::getCommonPythonPaths(std::vector<boost::filesystem::path> & paths)
{
   Poco::Util::WinRegistryKey check32(HKEY_LOCAL_MACHINE, "SOFTWARE\\Python\\PythonCore\\2.7\\InstallPath", true);
   if (check32.exists())
   {
      std::string pythonPath = check32.getString("");
      if(!pythonPath.empty())
         paths.push_back(boost::filesystem::path(pythonPath));
   }

   Poco::Util::WinRegistryKey check64(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Python\\PythonCore\\2.7\\InstallPath", true);
   if (check64.exists())
   {
      std::string pythonPath = check64.getString("");
      if (!pythonPath.empty())
         paths.push_back(boost::filesystem::path(pythonPath));
   }
   
   paths.push_back(boost::filesystem::path("C:\\python27"));

   try
   {
      paths.push_back(boost::filesystem::path(Poco::Environment::get("ProgramFiles")) / "python27");
   }
   catch (Poco::NotFoundException&)
   {
   }

   try
   {
      paths.push_back(boost::filesystem::path(Poco::Environment::get("ProgramFiles(x86)")) / "python27");
   }
   catch (Poco::NotFoundException&)
   {
   }
}


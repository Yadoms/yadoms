#include "stdafx.h"
#include "PythonExecutablePath.h"
#include <Poco/Util/WinRegistryKey.h>


void CPythonExecutablePath::getCommonPythonPaths(std::vector<boost::filesystem::path>& paths)
{
   try
   {
      Poco::Util::WinRegistryKey check32(HKEY_LOCAL_MACHINE, "SOFTWARE\\Python\\PyLauncher", true);
      if (check32.exists())
      {
         const auto pythonPath = check32.getString("InstallDir");
         if (!pythonPath.empty())
            paths.push_back(boost::filesystem::path(pythonPath));
      }
   }
   catch (std::exception&)
   {
   }


   try
   {
      Poco::Util::WinRegistryKey check64(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Python\\PyLauncher", true);
      if (check64.exists())
      {
         const auto pythonPath = check64.getString("InstallDir");
         if (!pythonPath.empty())
            paths.push_back(boost::filesystem::path(pythonPath));
      }
   }
   catch (std::exception&)
   {
   }

   try
   {
      Poco::Util::WinRegistryKey pythonCore(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Python\\PythonCore", true);
      if (pythonCore.exists())
      {
         Poco::Util::WinRegistryKey::Keys installedPythonVersions;
         pythonCore.subKeys(installedPythonVersions);
         for (auto& installedPythonVersion : installedPythonVersions)
         {
            if (installedPythonVersion.find("3.7") != std::string::npos)
            {
               Poco::Util::WinRegistryKey currentversion(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Python\\PythonCore\\" + installedPythonVersion, true);
               if (currentversion.exists())
               {
                  Poco::Util::WinRegistryKey::Keys subdirs;
                  currentversion.subKeys(subdirs);
                  try
                  {
                     if (std::find(subdirs.begin(), subdirs.end(), "InstallDir") != subdirs.end())
                     {
                        Poco::Util::WinRegistryKey installPath(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Python\\PythonCore\\" + installedPythonVersion + "\\InstallDir", true);
                        if (installPath.exists())
                        {
                           const auto pythonPath = installPath.getString("");
                           if (!pythonPath.empty())
                              paths.push_back(boost::filesystem::path(pythonPath));
                        }
                     }
                  }
                  catch (std::exception&)
                  {
                  }

                  try
                  {
                     if (std::find(subdirs.begin(), subdirs.end(), "InstallPath") != subdirs.end())
                     {
                        
                        Poco::Util::WinRegistryKey installPath(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Python\\PythonCore\\" + installedPythonVersion + "\\InstallPath", true);
                        if (installPath.exists())
                        {
                           const auto pythonPath = installPath.getString(""); //default value
                           if (!pythonPath.empty())
                              paths.push_back(boost::filesystem::path(pythonPath));
                        }
                     }
                  }
                  catch (std::exception&)
                  {
                  }
               }
            }

         }
      }
   }
   catch (std::exception&)
   {
   }
   //try another standard path
   paths.push_back(boost::filesystem::path("C:\\python37"));

   // Remove duplicates
   std::sort(paths.begin(), paths.end());
   paths.erase(std::unique(paths.begin(), paths.end()), paths.end());
}

const std::string& CPythonExecutablePath::getExecutableName()
{
   static const std::string ExecutableName("python"); //without extension
   return ExecutableName;
}

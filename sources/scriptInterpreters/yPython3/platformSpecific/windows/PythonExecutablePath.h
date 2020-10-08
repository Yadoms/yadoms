#pragma once

#include <Poco/Util/WinRegistryKey.h>

//--------------------------------------------------------------
/// \brief	Utility class for platform specific code
//--------------------------------------------------------------
class CPythonExecutablePath
{
public:
   //--------------------------------------------------------------
   /// \brief	Get the common python3 paths
   /// \return paths The common python3 installed path list
   //--------------------------------------------------------------
   static boost::shared_ptr<std::vector<boost::filesystem::path>> getCommonPythonPaths();

   //--------------------------------------------------------------
   /// \brief	Get the common python3 executable name
   /// \return paths The common ppython3 executable name
   //--------------------------------------------------------------
   static const std::string& getExecutableName();

private:

   static void getPyLauncherPath(const HKEY& hKey,
                                 boost::shared_ptr<std::vector<boost::filesystem::path>> paths);

   static void fillPythonPath(Poco::Util::WinRegistryKey& winRegistryKey,
                              boost::shared_ptr<std::vector<boost::filesystem::path>> paths,
                              bool defaultInstallDir = true);

   static void getPythonCorePath(const HKEY& hKey,
                                 boost::shared_ptr<std::vector<boost::filesystem::path>> paths);


   static void getPythonCorePath(Poco::Util::WinRegistryKey& winRegistryKey,
                                 const HKEY& hKey,
                                 boost::shared_ptr<std::vector<boost::filesystem::path>> paths);

   static void fillPythonPathByPyCore(const HKEY& hKey, const std::string& subKey,
                                      const std::string& installedPythonVersion,
                                      boost::shared_ptr<std::vector<boost::filesystem::path>> paths);


   const static std::string PyLauncher32BitPath;
   const static std::string PyLauncher64BitPath;
   const static std::string PythonCore32BitPath;
   const static std::string PythonCore64BitPath;
   const static std::string Python37StandardPath;
   const static std::string InstallDir;
   const static std::string InstallPath;
};

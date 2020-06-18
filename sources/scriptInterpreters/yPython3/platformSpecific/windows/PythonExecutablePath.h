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
   /// \param[out] paths The common python3 installed path list
   //--------------------------------------------------------------
   static void getCommonPythonPaths(std::vector<boost::filesystem::path>& paths);

   //--------------------------------------------------------------
   /// \brief	Get the common python3 executable name
   /// \return paths The common ppython3 executable name
   //--------------------------------------------------------------
   static const std::string& getExecutableName();

private:

   static void getPyLauncherPath(const HKEY& hKey,
                                 std::vector<boost::filesystem::path>& paths);

   static void fillPythonPath(Poco::Util::WinRegistryKey& winRegistryKey,
                              std::vector<boost::filesystem::path>& paths,
                              bool defaultInstallDir = true);

   static void getPythonCorePath(const HKEY& hKey,
                                 std::vector<boost::filesystem::path>& paths);


   static void getPythonCorePath(Poco::Util::WinRegistryKey& winRegistryKey,
                                 const HKEY& hKey,
                                 std::vector<boost::filesystem::path>& paths);

   static void fillPythonPathByPyCore(const HKEY& hKey, const std::string& subKey,
                                      const std::string& installedPythonVersion,
                                      std::vector<boost::filesystem::path>& paths);


   const static std::string PyLauncher32BitPath;
   const static std::string PyLauncher64BitPath;
   const static std::string PythonCore32BitPath;
   const static std::string PythonCore64BitPath;
   const static std::string Python37StandardPath;
   const static std::string InstallDir;
   const static std::string InstallPath;
};

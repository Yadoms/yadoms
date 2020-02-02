#include "stdafx.h"
#include "PythonExecutablePath.h"
#include <Poco/Util/WinRegistryKey.h>

const std::string CPythonExecutablePath::PyLauncher32BitPath("SOFTWARE\\Python\\PyLauncher");
const std::string CPythonExecutablePath::PyLauncher64BitPath("SOFTWARE\\Wow6432Node\\Python\\PyLauncher");
const std::string CPythonExecutablePath::PythonCore32BitPath("SOFTWARE\\Python\\PythonCore");
const std::string CPythonExecutablePath::PythonCore64BitPath("SOFTWARE\\Wow6432Node\\Python\\PythonCore");
const std::string CPythonExecutablePath::Python37StandardPath("C:\\python37");
const std::string CPythonExecutablePath::InstallDir("InstallDir");
const std::string CPythonExecutablePath::InstallPath("InstallPath");

void CPythonExecutablePath::getCommonPythonPaths(std::vector<boost::filesystem::path>& paths)
{
	try
	{
		getPyLauncherPath(HKEY_LOCAL_MACHINE, paths);
		getPyLauncherPath(HKEY_CURRENT_USER, paths);

		getPythonCorePath(HKEY_LOCAL_MACHINE, paths);
		getPythonCorePath(HKEY_CURRENT_USER, paths);

		//try another standard path
		paths.emplace_back(Python37StandardPath);

		// Remove duplicates
		std::sort(paths.begin(), paths.end());
		paths.erase(std::unique(paths.begin(), paths.end()), paths.end());
	}
	catch (std::exception&)
	{
	}
}

const std::string& CPythonExecutablePath::getExecutableName()
{
	static const std::string ExecutableName("python"); //without extension
	return ExecutableName;
}

void CPythonExecutablePath::getPyLauncherPath(const HKEY hKey, std::vector<boost::filesystem::path>& paths)
{
	Poco::Util::WinRegistryKey winRegistryKey32Bit(hKey, PyLauncher32BitPath, true);
	fillPythonPath(winRegistryKey32Bit, paths);

	Poco::Util::WinRegistryKey winRegistryKey64Bit(hKey, PyLauncher64BitPath, true);
	fillPythonPath(winRegistryKey64Bit, paths);
}

void CPythonExecutablePath::fillPythonPath(Poco::Util::WinRegistryKey& winRegistryKey,
                                           std::vector<boost::filesystem::path>& paths,
                                           const bool defaultInstallDir)
{
	if (winRegistryKey.exists())
	{
		const auto pythonPath = winRegistryKey.getString(defaultInstallDir ? InstallDir : "");
		if (!pythonPath.empty())
			paths.emplace_back(pythonPath);
	}
}

void CPythonExecutablePath::getPythonCorePath(const HKEY hKey, std::vector<boost::filesystem::path>& paths)
{
	Poco::Util::WinRegistryKey pythonCore32Bit(hKey, PythonCore32BitPath, true);

	Poco::Util::WinRegistryKey pythonCore64Bit(hKey, PythonCore64BitPath, true);

	if (pythonCore32Bit.exists())
	{
		getPythonCorePath(pythonCore32Bit, hKey, paths);
	}
	else if (pythonCore64Bit.exists())
	{
		getPythonCorePath(pythonCore64Bit, hKey, paths);
	}
}

void CPythonExecutablePath::getPythonCorePath(Poco::Util::WinRegistryKey& winRegistryKey,
                                              const HKEY& hKey,
                                              std::vector<boost::filesystem::path>& paths)
{
	Poco::Util::WinRegistryKey::Keys installedPythonVersions;
	winRegistryKey.subKeys(installedPythonVersions);
	for (auto& installedPythonVersion : installedPythonVersions)
	{
		if (!installedPythonVersion.empty())
		{
			fillPythonPathByPyCore(hKey, PythonCore32BitPath, installedPythonVersion, paths);
			fillPythonPathByPyCore(hKey, PythonCore64BitPath, installedPythonVersion, paths);
		}
	}
}

void CPythonExecutablePath::fillPythonPathByPyCore(const HKEY& hKey, const std::string& subKey,
                                                   std::string& installedPythonVersion,
                                                   std::vector<boost::filesystem::path>& paths)
{
	Poco::Util::WinRegistryKey currentVersion(
		hKey, subKey + "\\" + installedPythonVersion, true);
	if (currentVersion.exists())
	{
		Poco::Util::WinRegistryKey::Keys subdirs;
		currentVersion.subKeys(subdirs);
		try
		{
			if (std::find(subdirs.begin(), subdirs.end(), InstallDir) != subdirs.end())
			{
				Poco::Util::WinRegistryKey installPath(
					hKey,
					subKey + "\\" + installedPythonVersion + "\\" + InstallDir,
					true);

				fillPythonPath(installPath, paths, false);
			}
		}
		catch (std::exception&)
		{
		}

		try
		{
			if (std::find(subdirs.begin(), subdirs.end(), InstallPath) != subdirs.end())
			{
				Poco::Util::WinRegistryKey installPath(
					hKey,
					subKey + "\\" + installedPythonVersion + "\\" + InstallPath,
					true);
				fillPythonPath(installPath, paths, false);
			}
		}
		catch (std::exception&)
		{
		}
	}
}

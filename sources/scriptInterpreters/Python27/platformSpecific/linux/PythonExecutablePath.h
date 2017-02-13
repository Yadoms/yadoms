#pragma once


//--------------------------------------------------------------
/// \brief	Utility class for platform specific code
//--------------------------------------------------------------
class CPythonExecutablePath
{
public:
   //--------------------------------------------------------------
   /// \brief	Get the common python.exe paths
   /// \param[out] paths The common python.exe installed path
   //--------------------------------------------------------------
   static void getCommonPythonPaths(std::vector<boost::filesystem::path> & paths);
};

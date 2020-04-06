#pragma once


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
   static void getCommonPythonPaths(std::vector<boost::filesystem::path> & paths);

   //--------------------------------------------------------------
   /// \brief	Get the common python3 executable name
   /// \return paths The common ppython3 executable name
   //--------------------------------------------------------------
   static const std::string& getExecutableName();
};

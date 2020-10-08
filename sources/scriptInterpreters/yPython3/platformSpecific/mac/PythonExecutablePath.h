#pragma once


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
};

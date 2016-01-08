#pragma once


//--------------------------------------------------------------
/// \brief	Utility class for file path
//--------------------------------------------------------------
class CFileSystemPathHelper
{
public:
   //--------------------------------------------------------------
   /// \brief	Get the path of the library/executable which contains this code
   /// \param[out] paths The path
   //--------------------------------------------------------------
    static boost::filesystem::path getExecutingPath();
};

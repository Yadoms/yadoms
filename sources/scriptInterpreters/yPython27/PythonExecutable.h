#pragma once
#include "IPythonExecutable.h"

//--------------------------------------------------------------
/// \brief	Python executable
//--------------------------------------------------------------
class CPythonExecutable : public IPythonExecutable
{
public:
   explicit CPythonExecutable(const std::string& pythonForcedPath);
   virtual ~CPythonExecutable() = default;

protected:
   // IPythonExecutable Implementation
   bool found() const override;
   std::string version() const override;
   bool inSystemPath() const override;
   boost::filesystem::path path() const override;
   std::string filename() const override;
   // [END] IPythonExecutable Implementation

   //--------------------------------------------------------------
   /// \brief	Find the Python executable directory 
   /// \param[in] pythonForcedPath The Python forced path (empty if path should be auto-detected)
   /// \param[out] pythonDirectory Found Python directory (empty if Python is in the system path)
   /// \param[out] inSystemPath True if found in system path
   /// \return false if not found
   //--------------------------------------------------------------
   static bool findPythonDirectory(const std::string& pythonForcedPath,
                                   boost::filesystem::path& pythonDirectory,
                                   bool& inSystemPath);

   //--------------------------------------------------------------
   /// \brief	Check if Python is found in a directory
   /// \param[in] directory Directory to look for Python (if empty, look in the system path)
   /// \param[out] pythonDirectory Found Python directory (empty if Python is in the system path). Not filled if not found.
   /// \param[out] inSystemPath True if found in system path
   /// \return true if Python found in this directory
   //--------------------------------------------------------------
   static bool isPythonIn(const boost::filesystem::path& directory,
                          boost::filesystem::path& pythonDirectory,
                          bool& inSystemPath);

   //--------------------------------------------------------------
   /// \brief	Read the installed Python version (non-static form)
   /// \param[in] initialDirectory Directory from where to call Python executable
   /// \return string containing the Python version, as returned by Python (probably something like "Python 2.7.9")
   //--------------------------------------------------------------
   static std::string readPythonVersion(const boost::filesystem::path& initialDirectory);

private:
   //--------------------------------------------------------------
   /// \brief	The Python executable directory
   /// \note Empty if Python executable was not found
   //--------------------------------------------------------------
   boost::filesystem::path m_executableDirectory;

   //--------------------------------------------------------------
   /// \brief	Python executable is in system path
   //--------------------------------------------------------------
   bool m_inSystemPath;

   //--------------------------------------------------------------
   /// \brief	Python executable found flag
   //--------------------------------------------------------------
   const bool m_found;

   //--------------------------------------------------------------
   /// \brief	Installed Python version
   /// \details string containing the Python version, as returned by Python (something like "Python 2.7.9")
   //--------------------------------------------------------------
   const std::string m_version;
};


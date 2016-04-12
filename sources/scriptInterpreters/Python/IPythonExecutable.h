#pragma once

//--------------------------------------------------------------
/// \brief	Python executable interface
//--------------------------------------------------------------
class IPythonExecutable
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IPythonExecutable() {}

   //--------------------------------------------------------------
   /// \brief	To know if Python interpreter was found in the system
   /// \return true if found
   //--------------------------------------------------------------
   virtual bool found() const = 0;

   //--------------------------------------------------------------
   /// \brief	Installed Python version
   /// \details string containing the Python version, as returned by Python (something like "Python 2.7.9")
   //--------------------------------------------------------------
   virtual std::string version() const = 0;

   //--------------------------------------------------------------
   /// \brief	Get executable path
   /// \return the full (relative) Python executable path
   //--------------------------------------------------------------
   virtual boost::filesystem::path path() const = 0;

   //--------------------------------------------------------------
   /// \brief	Get executable filename
   /// \return the Python executable filename (with extension)
   //--------------------------------------------------------------
   virtual std::string filename() const = 0;
};

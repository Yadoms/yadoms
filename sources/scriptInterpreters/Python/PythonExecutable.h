#pragma once
#include "IPythonExecutable.h"

//--------------------------------------------------------------
/// \brief	Python executable
//--------------------------------------------------------------
class CPythonExecutable : public IPythonExecutable
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CPythonExecutable();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CPythonExecutable();

protected:
   // IPythonExecutable Implementation
   virtual bool found() const;
   virtual std::string version() const;
   virtual void startModule(boost::shared_ptr<const IScriptFile> scriptFile, const std::string& contextAccessorId, boost::shared_ptr<shared::script::ILogger> scriptLogger);
   virtual void interrupt();
   virtual int waitForStop();
   // [END] IPythonExecutable Implementation

protected:
   //--------------------------------------------------------------
   /// \brief	Find the Python executable directory
   /// \param[out] pythonDirectory Found Python directory (empty if Python is in the system path)
   /// \return false if not found
   //--------------------------------------------------------------
   static bool findPythonDirectory(boost::filesystem::path& pythonDirectory);

   //--------------------------------------------------------------
   /// \brief	Read the installed Python version (non-static form)
   /// \param[in] initialDirectory Directory from where to call Python executable
   /// \return string containing the Python version, as returned by Python (probably something like "Python 2.7.9")
   //--------------------------------------------------------------
   static std::string readPythonVersion(const boost::filesystem::path& initialDirectory);

   //--------------------------------------------------------------
   /// \brief	Thread redirecting standard outputs
   /// \param[in] ruleName          Rule name used for log identification
   /// \param[in] moduleStdOut      StdOut to redirect
   /// \param[in] targetStream      Target stream
   //--------------------------------------------------------------
   void stdRedirectWorker(const std::string& ruleName,
      boost::shared_ptr<Poco::PipeInputStream> moduleStdOut, boost::shared_ptr<shared::script::ILogger> scriptLogger);

private:
   //--------------------------------------------------------------
   /// \brief	The Python executable directory
   /// \note Empty if Python executable was not found
   //--------------------------------------------------------------
   boost::filesystem::path m_executableDirectory;

   //--------------------------------------------------------------
   /// \brief	Python executable found flag
   //--------------------------------------------------------------
   const bool m_found;

   //--------------------------------------------------------------
   /// \brief	Installed Python version
   /// \details string containing the Python version, as returned by Python (something like "Python 2.7.9")
   //--------------------------------------------------------------
   const std::string m_version;

   //--------------------------------------------------------------
   /// \brief	The process of the running script, and its mutex
   //--------------------------------------------------------------
   boost::shared_ptr<Poco::ProcessHandle> m_process;
   mutable boost::recursive_mutex m_processMutex;

   //--------------------------------------------------------------
   /// \brief	Thread redirecting standard outputs
   //--------------------------------------------------------------
   boost::thread m_StdOutRedirectingThread;
   boost::thread m_StdErrRedirectingThread;
};




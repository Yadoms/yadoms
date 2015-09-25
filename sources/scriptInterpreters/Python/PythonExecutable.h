#pragma once
#include "IPythonExecutable.h"
#include <shared/ThreadBase.h>
#include <shared/Log.h>//TODO utile ?

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
   virtual void waitForStop();
   virtual void interrupt();
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
   /// \brief	            Filter some return codes, which are not really errors
   /// \param[in] code     Return code (from call to Poco::ProcessHandle::wait)
   /// \return             true if it is a real error, false if not
   //--------------------------------------------------------------
   bool isError(int code) const;

   //--------------------------------------------------------------
   /// \brief	Thread redirecting standard outputs
   /// \param[in] moduleStdOut      StdOut to redirect
   /// \param[in] targetStream      Target stream
   //--------------------------------------------------------------
   void stdRedirectWorker(boost::shared_ptr<Poco::PipeInputStream> moduleStdOut, boost::shared_ptr<shared::script::ILogger> scriptLogger);

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
   class CStdOutRedirectingThread : public shared::CThreadBase//TODO virer
   {
   public:
      CStdOutRedirectingThread(const std::string& threadName, boost::shared_ptr<Poco::PipeInputStream> moduleStdOut, boost::shared_ptr<shared::script::ILogger> scriptLogger)
         :shared::CThreadBase(threadName), m_moduleStdOut(moduleStdOut), m_scriptLogger(scriptLogger) {}
      virtual void doWork()
      {
         char line[1024];
         while (m_moduleStdOut->getline(line, sizeof(line)))
            m_scriptLogger->log(line);
         //{
//            ((boost::shared_ptr<CLogger>)m_scriptLogger)->setThreadName(getName());//TODO virer ce cast
//            YADOMS_LOG_CONFIGURE(getName());
            //m_scriptLogger->out() << "out ==> " << line << std::endl;
            //YADOMS_LOG(information) << "YADOMS_LOG ==> " << line << std::endl;
         //}
      }
   private:
      boost::shared_ptr<Poco::PipeInputStream> m_moduleStdOut;
      boost::shared_ptr<shared::script::ILogger> m_scriptLogger;
   };
   //boost::shared_ptr<CStdOutRedirectingThread> m_StdOutRedirectingThread;
   boost::thread m_StdOutRedirectingThread;
//   boost::thread m_StdErrRedirectingThread;
};




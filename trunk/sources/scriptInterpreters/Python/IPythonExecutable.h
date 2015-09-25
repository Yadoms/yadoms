#pragma once
#include <shared/script/ILogger.h>
#include "IScriptFile.h"

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
   /// \brief	Start a module (in separated process)
   /// \param[in] scriptFile The script file to call
   /// \param[in] contextAccessorId The context accessor ID, used by script to interact with Yadoms
   /// \param[in] scriptLogger The script logger
   /// \throw CPythonException if error
   //--------------------------------------------------------------
   virtual void startModule(boost::shared_ptr<const IScriptFile> scriptFile, const std::string& contextAccessorId, boost::shared_ptr<shared::script::ILogger> scriptLogger) = 0;

   //--------------------------------------------------------------
   /// \brief	Wait for module stop (blocking)
   /// \throw CPythonException if error
   //--------------------------------------------------------------
   virtual void waitForStop() = 0;

   //--------------------------------------------------------------
   /// \brief	Ask for module stop
   /// \throw CPythonException if error
   //--------------------------------------------------------------
   virtual void interrupt() = 0;
};

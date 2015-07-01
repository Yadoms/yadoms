#pragma once
#include "IPythonExecutable.h"
#include "IScriptfile.h"
#include <shared/script/IRunner.h>
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Python initializer interface (RAII support)
//--------------------------------------------------------------
class CRunner : public shared::script::IRunner
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   ///\param[in] scriptPath   Script path (without name)
   ///\param[in] interpreterPath       The interpreter path
   ///\param[in] executable            The Python executable access
   ///\param[in] scriptConfiguration   Configuration of the script (optional)
   /// \throw CRunnerException if unable to load script
   //--------------------------------------------------------------
   CRunner(const std::string& scriptPath, const std::string& interpreterPath, boost::shared_ptr<IPythonExecutable> executable,
      const shared::CDataContainer& scriptConfiguration = shared::CDataContainer());

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CRunner();

protected:
   // IRunner Implementation
   virtual void run(shared::script::yScriptApi::IYScriptApi& context, boost::shared_ptr<shared::script::ILogger> scriptLogger);
   virtual void interrupt();
   virtual bool isOk() const;
   virtual std::string error() const;
   // [END] IRunner Implementation

private:
   //--------------------------------------------------------------
   /// \brief	Script file
   //--------------------------------------------------------------
   boost::shared_ptr<const IScriptFile> m_scriptFile;

   //--------------------------------------------------------------
   ///\brief   Configuration of the script
   //--------------------------------------------------------------
   const shared::CDataContainer m_scriptConfiguration;
   
   //--------------------------------------------------------------
   ///\brief   Interpreter path
   //--------------------------------------------------------------
   const std::string m_interpreterPath;

   //--------------------------------------------------------------
   /// \brief	The Python executable accessor
   //--------------------------------------------------------------
   boost::shared_ptr<IPythonExecutable> m_executable;

   //--------------------------------------------------------------
   /// \brief	The process of the running script
   //--------------------------------------------------------------
   boost::shared_ptr<Poco::ProcessHandle> m_process;

   //--------------------------------------------------------------
   ///\brief   Last error message (empty if no error)
   //--------------------------------------------------------------
   std::string m_lastError;
};




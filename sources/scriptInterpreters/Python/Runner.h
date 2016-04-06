#pragma once
#include "IPythonExecutable.h"
#include "IScriptFile.h"
#include <shared/DataContainer.h>
#include <shared/process/ILogger.h>
#include <shared/process/IProcess.h>
#include <shared/process/IRunner.h>
#include <shared/process/IStopNotifier.h>
#include <shared/script/yScriptApi/IYScriptApi.h>
#include "IContextAccessor.h"

//--------------------------------------------------------------
/// \brief	Python initializer interface (RAII support)
//--------------------------------------------------------------
class CRunner : public shared::process::IRunner
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   ///\param[in] scriptPath            Script path (without name)
   ///\param[in] executable            The Python executable access
   ///\param[in] scriptLogger          The rule logger
   ///\param[in] yScriptApi            The rule context
   ///\param[in] stopNotifier          The stop notifier
   ///\param[in] scriptConfiguration   Configuration of the script
   ///\param[in] scriptConfiguration   Configuration of the script (optional)
   /// \throw CRunnerException if unable to load script
   //--------------------------------------------------------------
   CRunner(const std::string& scriptPath,
      boost::shared_ptr<IPythonExecutable> executable,
      boost::shared_ptr<shared::process::ILogger> scriptLogger,
      boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi,
      boost::shared_ptr<shared::process::IStopNotifier> stopNotifier,
      const shared::CDataContainer& scriptConfiguration = shared::CDataContainer());

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CRunner();

   // shared::process::IRunner Implementation
   void requestStop();
   // [END] shared::process::IRunner Implementation

protected:
   //-----------------------------------------------------
   ///\brief               Start the rule
   //-----------------------------------------------------
   void start();

   //-----------------------------------------------------
   ///\brief               Rule montoring thread function
   ///\param[in] process      The process to monitor
   ///\param[in] stopNotifier The rule stop notifier
   ///\param[in] scriptLogger The rule script logger
   //-----------------------------------------------------
   static void monitorThreaded(boost::shared_ptr<shared::process::IProcess> process,
      boost::shared_ptr<shared::process::IStopNotifier> stopNotifier,
      boost::shared_ptr<shared::process::ILogger> scriptLogger);

private:
   //--------------------------------------------------------------
   /// \brief	Script file
   //--------------------------------------------------------------
   boost::shared_ptr<const IScriptFile> m_scriptFile;

   //--------------------------------------------------------------
   /// \brief	The Python executable accessor
   //--------------------------------------------------------------
   boost::shared_ptr<IPythonExecutable> m_executable;

   //--------------------------------------------------------------
   ///\brief   The rule logger
   //--------------------------------------------------------------
   boost::shared_ptr<shared::process::ILogger> m_scriptLogger;

   //--------------------------------------------------------------
   ///\brief   The rule context
   //--------------------------------------------------------------
   boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> m_scriptApi;

   //--------------------------------------------------------------
   ///\brief   The rule stop notifier
   //--------------------------------------------------------------
   boost::shared_ptr<shared::process::IStopNotifier> m_stopNotifier;
      
   //--------------------------------------------------------------
   ///\brief   Configuration of the script
   //--------------------------------------------------------------
   const shared::CDataContainer m_scriptConfiguration;

   //--------------------------------------------------------------
   ///\brief   The context accessor (gateway from script message queue and scriptApi)
   //--------------------------------------------------------------
   boost::shared_ptr<IContextAccessor> m_contextAccessor;

   //--------------------------------------------------------------
   ///\brief   The process
   //--------------------------------------------------------------
   boost::shared_ptr<shared::process::IProcess> m_process;

   //--------------------------------------------------------------
   /// \brief	Thread monitoring the rule
   //--------------------------------------------------------------
   boost::thread m_monitor;
};




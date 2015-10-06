#pragma once
#include "IPythonExecutable.h"
#include "IScriptFile.h"
#include <shared/DataContainer.h>
#include <shared/script/IRunner.h>
#include <shared/script/IStopNotifier.h>
#include "IContextAccessor.h"

//--------------------------------------------------------------
/// \brief	Python initializer interface (RAII support)
//--------------------------------------------------------------
class CRunner : public shared::script::IRunner
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
      boost::shared_ptr<shared::script::ILogger> scriptLogger,
      boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi,
      boost::shared_ptr<shared::script::IStopNotifier> stopNotifier,
      const shared::CDataContainer& scriptConfiguration = shared::CDataContainer());

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CRunner();

protected:
   // IRunner Implementation
   virtual bool isOk() const;
   virtual std::string error() const;
   // [END] IRunner Implementation

   //-----------------------------------------------------
   ///\brief               Start the rule
   //-----------------------------------------------------
   void start();

   //-----------------------------------------------------
   ///\brief               Stop the rule
   //-----------------------------------------------------
   void stop();

   //-----------------------------------------------------
   ///\brief               Rule montoring thread function
   ///\param[in] stopNotifier The rule stop notifier
   //-----------------------------------------------------
   void monitor(boost::shared_ptr<shared::script::IStopNotifier> stopNotifier);

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
   boost::shared_ptr<shared::script::ILogger> m_scriptLogger;

   //--------------------------------------------------------------
   ///\brief   The rule context
   //--------------------------------------------------------------
   boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> m_scriptApi;

   //--------------------------------------------------------------
   ///\brief   The rule stop notifier
   //--------------------------------------------------------------
   boost::shared_ptr<shared::script::IStopNotifier> m_stopNotifier;
      
   //--------------------------------------------------------------
   ///\brief   Configuration of the script
   //--------------------------------------------------------------
   const shared::CDataContainer m_scriptConfiguration;

   //--------------------------------------------------------------
   ///\brief   The context accessor (gateway from script message queue and scriptApi)
   //--------------------------------------------------------------
   boost::shared_ptr<IContextAccessor> m_contextAccessor;

   //--------------------------------------------------------------
   ///\brief   Last error message (empty if no error)
   //--------------------------------------------------------------
   std::string m_lastError;

   //--------------------------------------------------------------
   /// \brief	Thread monitoring the rule
   //--------------------------------------------------------------
   boost::thread m_monitor;
};




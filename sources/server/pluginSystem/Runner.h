#pragma once
#include <shared/DataContainer.h>
#include <shared/process/IRunner.h>
#include <shared/process/IStopNotifier.h>
#include <shared/script/yScriptApi/IYScriptApi.h>
#include <shared/process/IProcess.h>
#include <shared/process/ILogger.h>
#include "IInstanceStateHandler.h"
#include <shared/process/ILogger.h>

namespace pluginSystem {

   //--------------------------------------------------------------
   /// \brief	Python initializer interface (RAII support)
   //--------------------------------------------------------------
   class CRunner : public shared::process::IRunner
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      ///\param[in] process               The associated process
      ///\param[in] logger                The logger
      ///\param[in] stopNotifier          The stop notifier
      //--------------------------------------------------------------
      CRunner(boost::shared_ptr<shared::process::IProcess> process,
         boost::shared_ptr<shared::process::ILogger> logger,
         boost::shared_ptr<IInstanceStateHandler> stopNotifier);

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
      boost::shared_ptr<shared::process::IProcess> m_process;
      boost::shared_ptr<shared::process::ILogger> m_logger;
      boost::shared_ptr<IInstanceStateHandler> m_stopNotifier;

      //--------------------------------------------------------------
      ///\brief   The process
      //--------------------------------------------------------------
      boost::shared_ptr<shared::process::IProcess> m_process;

      //--------------------------------------------------------------
      /// \brief	Thread monitoring the rule
      //--------------------------------------------------------------
      boost::thread m_monitor;
   };

} // namespace pluginSystem


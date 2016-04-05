#pragma once
#include <shared/process/IRunner.h>
#include <shared/process/IProcess.h>
#include "IInstanceStateHandler.h"

namespace pluginSystem
{
   //TODO rechercher la chaîne "rule" dans tout le répertoire pluginSystem et la virer

   //--------------------------------------------------------------
   /// \brief	Python initializer interface (RAII support)
   //--------------------------------------------------------------
   class CRunner : public shared::process::IRunner
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      ///\param[in] process               The associated process
      ///\param[in] instanceStateHandler  The instance state handler
      //--------------------------------------------------------------
      CRunner(boost::shared_ptr<shared::process::IProcess> process,
              boost::shared_ptr<IInstanceStateHandler> instanceStateHandler);

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
      //-----------------------------------------------------
      static void monitorThreaded(boost::shared_ptr<shared::process::IProcess> process,
                                  boost::shared_ptr<IInstanceStateHandler> instanceStateHandler);

   private:
      //--------------------------------------------------------------
      ///\brief   The process
      //--------------------------------------------------------------
      boost::shared_ptr<shared::process::IProcess> m_process;

      //--------------------------------------------------------------
      ///\brief   The stop notifier
      //--------------------------------------------------------------
      boost::shared_ptr<IInstanceStateHandler> m_instanceStateHandler;

      //--------------------------------------------------------------
      /// \brief	Thread monitoring the plugin execution
      //--------------------------------------------------------------
      boost::thread m_monitor;
   };
} // namespace pluginSystem


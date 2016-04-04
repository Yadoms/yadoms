#include "stdafx.h"
#include "Runner.h"

namespace pluginSystem
{
   CRunner::CRunner(boost::shared_ptr<shared::process::IProcess> process,
                    boost::shared_ptr<IInstanceStateHandler> instanceStateHandler)
      : m_process(process), m_instanceStateHandler(instanceStateHandler)
   {
      start();
   }

   CRunner::~CRunner()
   {
      m_monitor.join();
   }

   void CRunner::start()
   {
      m_monitor = boost::thread(&CRunner::monitorThreaded, m_process, m_instanceStateHandler);
   }


   void CRunner::requestStop()
   {
      if (m_process)
         m_process->interrupt();
   }

   void CRunner::monitorThreaded(boost::shared_ptr<shared::process::IProcess> process,
                                 boost::shared_ptr<IInstanceStateHandler> instanceStateHandler)
   {
      if (process->waitForStop() == 0)
         instanceStateHandler->signalNormalStop();
      else
         instanceStateHandler->signalError(process->getError());
   }
} // namespace pluginSystem


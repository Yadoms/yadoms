#include "stdafx.h"
#include "Runner.h"
#include "PluginException.hpp"
#include <shared/DataContainer.h>
#include "Process.h"
#include "PluginException.hpp"

namespace pluginSystem {

CRunner::CRunner(boost::shared_ptr<shared::process::IProcess> process,
                 boost::shared_ptr<shared::process::ILogger> logger,
                 boost::shared_ptr<IInstanceStateHandler> stopNotifier)
   :m_process(process), m_logger(logger), m_stopNotifier(stopNotifier)
{
   start();
}

CRunner::~CRunner()
{
   m_monitor.join();
}

void CRunner::start()
{
   m_monitor = boost::thread(&CRunner::monitorThreaded, m_process, m_stopNotifier, m_logger);
}


void CRunner::requestStop()
{
   if (m_process)
      m_process->interrupt();
}

void CRunner::monitorThreaded(
   boost::shared_ptr<shared::process::IProcess> process,
   boost::shared_ptr<shared::process::IStopNotifier> stopNotifier,
   boost::shared_ptr<shared::process::ILogger> scriptLogger)
{
   if (process->waitForStop() == 0)
      stopNotifier->notifyNormalStop();
   else
      stopNotifier->notifyError(process->getError());
}

} // namespace pluginSystem
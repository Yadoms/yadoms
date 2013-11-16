#pragma once

#include "../plugins/hardware/IHardwarePlugin.h"
#include <boost/thread.hpp>

class CHardwarePluginInstance
{
public:
   CHardwarePluginInstance(IHardwarePlugin * plugin);

   void start();

   void stop();

   void doWork();

private:
   IHardwarePlugin * m_pPlugin;
   boost::shared_ptr<boost::thread> m_thread;
};

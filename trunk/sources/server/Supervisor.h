#pragma once

#include "tools/ThreadBase.h"
#include "pluginSystem/HardwarePluginManager.h"
#include "StartupOptions.h"

class CSupervisor : public CThreadBase
{
public:
   CSupervisor(const IStartupOptions& startupOptions);
   virtual ~CSupervisor(void);

   void doWork();
private:
   const IStartupOptions& m_startupOptions;
};


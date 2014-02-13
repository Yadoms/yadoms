#pragma once

#include <shared/ThreadBase.h>
#include <shared/Event/EventHandler.hpp>
#include "StartupOptions/StartupOptions.h"

class CSupervisor : public CThreadBase, public CEventHandler
{
private:
   // Event IDs
   enum
   {
      kHardwarePluginManagerEvent = CEventHandler::kUserFirstId,
   };

public:
   CSupervisor(const IStartupOptions& startupOptions);
   virtual ~CSupervisor(void);

   // CThreadBase Implementation
   void doWork();
   // [END] CThreadBase Implementation

private:
   const IStartupOptions& m_startupOptions;
};


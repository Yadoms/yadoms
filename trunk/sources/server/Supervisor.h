#pragma once

#include <shared/ThreadBase.h>
#include <shared/event/EventHandler.hpp>
#include "startupOptions/IStartupOptions.h"

class CSupervisor : public shared::CThreadBase, public shared::event::CEventHandler
{
private:
   // Event IDs
   enum
   {
      kPluginManagerEvent = shared::event::CEventHandler::kUserFirstId,
   };

public:
   CSupervisor(const startupOptions::IStartupOptions& startupOptions);
   virtual ~CSupervisor(void);

   // CThreadBase Implementation
   void doWork();
   // [END] CThreadBase Implementation

private:
   const startupOptions::IStartupOptions& m_startupOptions;
};


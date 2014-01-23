#pragma once

#include <shared/ThreadBase.h>
#include "StartupOptions/StartupOptions.h"

class CSupervisor : public CThreadBase
{
public:
   CSupervisor(const IStartupOptions& startupOptions);
   virtual ~CSupervisor(void);

   // CThreadBase Implementation
   void doWork();
   // [END] CThreadBase Implementation

private:
   const IStartupOptions& m_startupOptions;
};


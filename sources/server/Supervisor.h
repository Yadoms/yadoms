#pragma once

#include "../tools/ThreadBase.h"
#include "PluginSystem/HardwarePluginManager.h"

class CSupervisor : public CThreadBase
{
public:
	CSupervisor(void);
	virtual ~CSupervisor(void);

	void doWork();
private:
    CHardwarePluginManager m_hardwarePluginManager;
};


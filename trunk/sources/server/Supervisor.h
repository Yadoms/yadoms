#pragma once

#include "../tools/ThreadBase.h"

class CSupervisor : public CThreadBase
{
public:
	CSupervisor(void);
	virtual ~CSupervisor(void);

	void doWork();
};


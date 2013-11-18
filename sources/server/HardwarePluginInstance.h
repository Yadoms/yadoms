#pragma once

#include "../plugins/hardware/IHardwarePlugin.h"
#include <boost/thread.hpp>
#include "../tools/ThreadBase.h"

class CHardwarePluginInstance : public CThreadBase
{
public:
   CHardwarePluginInstance(plugins::IHardwarePlugin * plugin);

private:
   plugins::IHardwarePlugin * m_pPlugin;

protected:
	//--------------------------------------------------------------
	/// \brief			The main plugin work
	/// \return    	void
	//--------------------------------------------------------------
	virtual void doWork();
};

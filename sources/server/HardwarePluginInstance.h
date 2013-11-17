#pragma once

#include "../plugins/hardware/IHardwarePlugin.h"
#include <boost/thread.hpp>
#include "../tools/ThreadBase.h"

class CHardwarePluginInstance : CThreadBase
{
public:
   CHardwarePluginInstance(IHardwarePlugin * plugin);

   void start();

   void stop();

private:
	
   void doWorkInternal();
   IHardwarePlugin * m_pPlugin;

protected:
	//--------------------------------------------------------------
	/// \brief			The main plugin work
	/// \return    	void
	//--------------------------------------------------------------
	virtual void doWork();
};

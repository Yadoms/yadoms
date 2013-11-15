#include "HardwarePluginBase.h"

namespace plugins
{
	class CFakePlugin : public CHardwarePluginBase
	{  
	public:
		CFakePlugin()
			:CHardwarePluginBase("CFakePlugin")
		{
		}

		void init(const std::string & pluginAddress, const std::string & serverAddress)
		{
		}
		
		void doWork() 
		{
		}
	};
}

EXPOSE_HARDWARE_PLUGIN(CFakePlugin)

#include "IHardwarePlugin.hpp"

namespace plugins
{
	class CFakePlugin : public IHardwarePlugin
	{  
	public:
		CFakePlugin()
			:IHardwarePlugin("CFakePlugin")
		{
		}

		void init(const std::string & pluginAddress, const std::string & serverAddress)
		{
		}
		
		// A function to do something, so we can demonstrate the plugin
		std::string toString()
		{
			return std::string("Coming from awesome plugin");
		}

		void doWork() 
		{
		}
	};
}

EXPOSE_HARDWARE_PLUGIN(CFakePlugin)

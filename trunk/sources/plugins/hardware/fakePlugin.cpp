#include "IHardwarePlugin.hpp"

namespace plugins
{
	class FakePlugin : public IHardwarePlugin
	{  
	public:
		FakePlugin()
			:IHardwarePlugin("FakePlugin")
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

extern "C" 
{
	// Function to return an instance of a new AwesomePlugin object
	EXPORT_MAIN_FUNCTIONS plugins::IHardwarePlugin* construct()
	{
		return new plugins::FakePlugin();
	}

	EXPORT_MAIN_FUNCTIONS void destruct(plugins::IHardwarePlugin* pluginToDelete)
	{
		delete pluginToDelete;
	}  
}
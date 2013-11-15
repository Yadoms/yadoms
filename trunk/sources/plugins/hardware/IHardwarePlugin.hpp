#ifndef _PLUGIN_HPP_
#define _PLUGIN_HPP_

#include <string>
#include "../../tools/ThreadBase.h"

#ifdef WIN32
#define EXPORT_MAIN_FUNCTIONS __declspec(dllexport)
#else
#define EXPORT_MAIN_FUNCTIONS 
#endif

#define EXPOSE_HARDWARE_PLUGIN(pluginClassName) \
	extern "C"                                                                              \
	{                                                                                       \
		EXPORT_MAIN_FUNCTIONS plugins::IHardwarePlugin* construct()                         \
		{                                                                                   \
			return new plugins::pluginClassName();                                          \
		}                                                                                   \
                                                                                            \
		EXPORT_MAIN_FUNCTIONS void destruct(plugins::IHardwarePlugin* pluginToDelete)       \
		{                                                                                   \
			delete pluginToDelete;                                                          \
		}                                                                                   \
	}                                                                                       \


namespace plugins
{
  class IHardwarePlugin : public CThreadBase
  {  
  public:
	IHardwarePlugin(const std::string & name)
		:CThreadBase(name)
	{
	}
	virtual void init(const std::string & pluginAddress, const std::string & serverAddress) =0;
    virtual std::string toString() = 0;
	
  };
}




#endif
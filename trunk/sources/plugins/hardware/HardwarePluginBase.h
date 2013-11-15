#ifndef _HARDWARE_PLUGIN_HPP_
#define _HARDWARE_PLUGIN_HPP_

#include <string>
#include "../../tools/ThreadBase.h"

#ifdef WIN32
#define EXPORT_MAIN_FUNCTIONS __declspec(dllexport)
#else
#define EXPORT_MAIN_FUNCTIONS 
#endif

#define EXPOSE_HARDWARE_PLUGIN(pluginClassName) \
	extern "C"                                                                             \
	{                                                                                      \
	   EXPORT_MAIN_FUNCTIONS plugins::CHardwarePluginBase* construct()                     \
		{                                                                                   \
		   return new plugins::pluginClassName();                                           \
		}                                                                                   \
		EXPORT_MAIN_FUNCTIONS void destruct(plugins::CHardwarePluginBase* pluginToDelete)   \
		{                                                                                   \
		   delete pluginToDelete;                                                           \
		}                                                                                   \
	}                                                                                      \


namespace plugins
{
	class CHardwarePluginBase : public CThreadBase
	{  
	public:
		CHardwarePluginBase(const std::string & name);
		virtual ~CHardwarePluginBase();
		virtual void init(const std::string & pluginAddress, const std::string & serverAddress) =0;
		virtual const std::string toString();
	};
}

#endif
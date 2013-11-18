#pragma once

#include <string>

namespace plugins
{
   class IHardwarePlugin
   {
   public:
      virtual ~IHardwarePlugin()
	  {
	  }

      // Do the work. This function runs in specific-thread context (yadoms main-thread independent).
      // Implementation should loop until end is asked by yadoms.
      // Be careful to put some sleeps to prevent using too much system ressources.
      // Use boost::this_thread::sleep is a watch stop point (a point where thread is able to stop).
      virtual void doWork() = 0;

   protected:
	  IHardwarePlugin()
	  {
	  }
   };
}


// TODO : move this block in a specific plateform header
#ifdef WIN32
#define EXPORT_MAIN_FUNCTIONS __declspec(dllexport)
#else
#define EXPORT_MAIN_FUNCTIONS 
#endif

// Plugin implementation helper
#define IMPLEMENT_HARDWARE_PLUGIN(pluginClassName) \
extern "C"                                                                             \
{                                                                                      \
   EXPORT_MAIN_FUNCTIONS plugins::IHardwarePlugin* construct()                         \
   {                                                                                   \
      return new plugins::pluginClassName();                                           \
   }                                                                                   \
   EXPORT_MAIN_FUNCTIONS void destruct(plugins::IHardwarePlugin* pluginToDelete)       \
   {                                                                                   \
      delete pluginToDelete;                                                           \
   }                                                                                   \
}   

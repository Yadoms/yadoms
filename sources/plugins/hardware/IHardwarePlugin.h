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
	  /*
	  //plus utile le constructeur est la pour ca
      // Called before DoWork function
      // Implementation of this function should start some system ressources
      virtual void start() = 0;

      // Called after DoWork function
      // Implementation of this function should free system ressources
      virtual void stop() = 0;*/

      // Do the work. This function runs in specific-thread context (yadoms main-thread independent).
      // Implementation should loop until end is asked by yadoms.
      // Be careful to put some sleeps to prevent using too much system ressources.
      // Use boost::this_thread::sleep is a watch stop point (a point where thread is able to stop).
      virtual void doWork() = 0;

	  //Return the information about the class under string format
	  virtual std::string toString() = 0;

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

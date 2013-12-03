
#include <boost/thread.hpp>
#include <boost/optional.hpp>
#include <boost/log/trivial.hpp>
#include <boost/assign.hpp>
#include "FakePlugin.h"


// Use this macro to define some basic informations about the plugin
IMPLEMENT_HARDWARE_PLUGIN(
   CFakePlugin,                                 // IHardwarePlugin implementation class
   "FakePlugin",                                // Plugin name (std::string)
   "0.1",                                       // Current plugin version (std::string)
   CHardwarePluginInformation::kBeta,           // Current release state (CHardwarePluginInformation::EReleaseType)
   "yadoms-team",                               // Author name (std::string)
   "http://sourceforge.net/projects/yadoms/")   // Url of author web site (std::string)


enum EEnumType
{
   kEnumValue1 = 7,
   kEnumValue2 = 12,
   kEnumValue3
};

// If parameters are needed for the plugin, just define the buidDefaultConfiguration
// function to add parameters to the plugin configuration, and call IMPLEMENT_CONFIGURATION macro
void buidDefaultConfiguration(CHardwarePluginConfiguration& configuration)
{
   static const CHardwarePluginConfigurationEnumParameter<EEnumType>::ValuesNames EEnumTypeNames = boost::assign::map_list_of
      (kEnumValue1, "EnumValue1")
      (kEnumValue2, "EnumValue2")
      (kEnumValue3, "EnumValue3");

   ADD_CONFIGURATION_PARAMETER_STRING("Serial port", "The serial port where my plugin is connected", "tty0");
   ADD_CONFIGURATION_PARAMETER_BOOL("BoolParameter", "Just a boolean example", false);
   ADD_CONFIGURATION_PARAMETER_ENUM(EEnumType, "EnumParameter", "Example of an enum with 3 values", kEnumValue2, EEnumTypeNames);
   ADD_CONFIGURATION_PARAMETER_INT("IntParameter", "This is my int parameter example", 7);
   ADD_CONFIGURATION_PARAMETER_DOUBLE("DoubleParameter", "Now a double", 25.3);
}
IMPLEMENT_CONFIGURATION;


CFakePlugin::CFakePlugin()
{
}

CFakePlugin::~CFakePlugin()
{
}

// IHardwarePlugin implementation

void CFakePlugin::doWork(const std::string& configurationValues)
{
   try
   {
      // Build configuration and load values from database
      CHardwarePluginConfiguration configuration=getDefaultConfiguration();
      configuration.unserializeValues(configurationValues);
      try
      {
         // Read parameter as string
         std::cout << "CFakePlugin::doWork, parameter 'Serial port' is " << configuration["Serial port"].valueToString() << std::endl;//TODO : En attendant que le log marche
         std::cout << "CFakePlugin::doWork, parameter 'BoolParameter' is " << configuration["BoolParameter"].valueToString() << std::endl;//TODO : En attendant que le log marche
         std::cout << "CFakePlugin::doWork, parameter 'EnumParameter' is " << configuration["EnumParameter"].valueToString() << std::endl;//TODO : En attendant que le log marche

         // More usable form : read parameter as its basic type
         if (configuration.asBool("BoolParameter"))
            std::cout << "CFakePlugin::doWork, parameter 'BoolParameter' is true" << std::endl;//TODO : En attendant que le log marche
         else
            std::cout << "CFakePlugin::doWork, parameter 'BoolParameter' is false" << std::endl;//TODO : En attendant que le log marche
         std::cout << "CFakePlugin::doWork, parameter 'Serial port' is " << configuration.asString("Serial port") << std::endl;
         std::cout << "CFakePlugin::doWork, parameter 'EnumParameter' is ";
         switch (configuration.asEnum<EEnumType>("EnumParameter"))
         {
         case kEnumValue1: std::cout << "EnumValue1"; break;
         case kEnumValue2: std::cout << "EnumValue2"; break;
         case kEnumValue3: std::cout << "EnumValue3"; break;
         default: std::cout << "Invalid value"; break;
         }
         std::cout << std::endl;
      }
      catch (const std::bad_cast& bc)
      {
         BOOST_ASSERT(false);  // Parameter is wrong type
         std::cerr << "Bad cast error: " << bc.what() << '\n';//TODO : log
      }
      catch (const std::out_of_range& oor)
      {
         BOOST_ASSERT(false);  // Parameter doesn't exist
         std::cerr << "Out of Range error: " << oor.what() << '\n';//TODO : log
      }

	   while(1)
	   {
	      BOOST_LOG_TRIVIAL(info) << "CFakePlugin is running...";
	
	      // Give a chance to exit plugin thread
         boost::this_thread::disable_interruption ri;
	      boost::this_thread::sleep(boost::posix_time::milliseconds(1000)); 
	   };
   }
   // Plugin must catch this end-of-thread exception
   catch (boost::thread_interrupted&)
   {
      BOOST_LOG_TRIVIAL(info) << "CFakePlugin is stopped...";
   }
}

void CFakePlugin::updateConfiguration(const std::string& configurationValues)
{
   //TODO : faire un exemple
   //TODO : faire une API "event", voir http://stackoverflow.com/questions/5598890/boost-equivelent-for-windows-events
}


// [END] IHardwarePlugin implementation


// TODO : WhatTheFuck ? ? ? C'est quoi ces adresses ?
void CFakePlugin::init(const std::string & pluginAddress, const std::string & serverAddress)
{
}


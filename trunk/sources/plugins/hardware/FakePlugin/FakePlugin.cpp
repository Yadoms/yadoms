
#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>
#include "FakePlugin.h"


// Use this macro to define some basic informations about the plugin
IMPLEMENT_HARDWARE_PLUGIN(
   CFakePlugin,                                 // IHardwarePlugin implementation class
   "FakePlugin",                                // Plugin name (std::string)
   "0.1",                                       // Current plugin version (std::string)
   CHardwarePluginInformation::kBeta,           // Current release state (CHardwarePluginInformation::EReleaseType)
   "yadoms-team",                               // Author name (std::string)
   "http://sourceforge.net/projects/yadoms/")   // Url of author web site (std::string)


// Use this macro to implement easy configuration for the plugin
// Next add parameters using the IMPLEMENT_HARDWARE_PLUGIN_ADD_EASYCONF_PARAMETER macro
//IMPLEMENT_HARDWARE_PLUGIN_EASYCONF

// Use this macro to add a parameter in the easy configuration
//TODO IMPLEMENT_HARDWARE_PLUGIN_ADD_EASYCONF_PARAMETER(
//   "Serial port",                               // Parameter name (std::string)
//   kInt,                                        // Parameter type
//   "tty0",                                      // (std::string value, convertible to the parameter type)
//   "")                                          // Enum values as std::string comma-separated list (only if parameter type = kEnum)
//
//IMPLEMENT_HARDWARE_PLUGIN_ADD_EASYCONF_PARAMETER(
//   "BoolParameter",
//   kBool,
//   "false",
//   "")


//--------------------------------------------------------------
/// \brief		Configuration description
/// \note      Use this syntax to define the plugin parameters, modifiable by user
///            - Parameter name is std::string
///            - Parameter type is CHardwarePluginConfigurationSchemaParameter::EParameterType
///            - Parameter default value is std::string value, convertible to the parameter type
///            - Enum values is std::string comma-separated list (only if parameter type = kEnum)
//--------------------------------------------------------------
static const CHardwarePluginConfigurationSchemaParameter EasyConfParameters[] = 
{  // Name           ,                 Type                                , default value, enum values
   { "Serial port"   , CHardwarePluginConfigurationSchemaParameter::kString, "tty0"       , ""                                   },
   { "BoolParameter" , CHardwarePluginConfigurationSchemaParameter::kBool  , "false"      , ""                                   },
   { "EnumParameter" , CHardwarePluginConfigurationSchemaParameter::kEnum  , "EnumValue1" , "EnumValue1;EnumValue2;EnumValue3"   }
};

CFakePlugin::CFakePlugin()
{
}

CFakePlugin::~CFakePlugin()
{
}

// IHardwarePlugin implementation

void CFakePlugin::doWork(const IHardwarePluginConfigurationProvider& config)
{
   try
   {
      // Get configuration from database
      std::map<std::string, CHardwarePluginConfigurationSchemaParameter> pluginParameters = config.get();
      if (pluginParameters.find("Serial port") != pluginParameters.end())
         BOOST_LOG_TRIVIAL(info) << "CFakePlugin parameter 'Serial port' is " << pluginParameters["Serial port"].m_defaultValue;//TODO : c'est pas top de manipuler m_defaultValue
      if (pluginParameters.find("BoolParameter") != pluginParameters.end())
         BOOST_LOG_TRIVIAL(info) << "CFakePlugin parameter 'BoolParameter' is " << pluginParameters["BoolParameter"].m_defaultValue;
      if (pluginParameters.find("EnumParameter") != pluginParameters.end())
         BOOST_LOG_TRIVIAL(info) << "CFakePlugin parameter 'EnumParameter' is " << pluginParameters["EnumParameter"].m_defaultValue;

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

// [END] IHardwarePlugin implementation


// TODO : WhatTheFuck ? ? ? C'est quoi ces adresses ?
void CFakePlugin::init(const std::string & pluginAddress, const std::string & serverAddress)
{
}


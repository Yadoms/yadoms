#include "stdafx.h"
#include "FakePlugin.h"
#include <shared/Log.h>
#include <shared/Xpl/XplService.h>
#include <shared/Xpl/XplMessage.h>
#include <shared/Exceptions/BadConversionException.hpp>

// This include is only needed if plugin needs configuration support
#include <shared/HardwarePlugin/Configuration/Configuration.h>


// Use this macro to define some basic informations about the plugin
IMPLEMENT_HARDWARE_PLUGIN(
   CFakePlugin,                                 // IHardwarePlugin implementation class
   "FakePlugin",                                // Plugin name (std::string)
   "0.1",                                       // Current plugin version (std::string)
   IHardwarePluginInformation::kBeta,           // Current release state (IHardwarePluginInformation::EReleaseType)
   "yadoms-team",                               // Author name (std::string)
   "http://sourceforge.net/projects/yadoms/")   // Url of author web site (std::string)


enum EEnumType
{
   kEnumValue1 = 7,
   kEnumValue2 = 12,
   kEnumValue3
};

// If parameters are needed for the plugin, just use the IMPLEMENT_CONFIGURATION macro
// to create the configuration schema and add parameters
IMPLEMENT_CONFIGURATION
{
   // Simple types
   ADD_CONFIGURATION_PARAMETER_STRING("StringParameter", "Just a string parameter", "Yadoms is so powerful !");
   ADD_CONFIGURATION_PARAMETER_BOOL("BoolParameter", "Just a boolean example", false);
   ADD_CONFIGURATION_PARAMETER_INT("IntParameter", "This is my int parameter example", 7);
   ADD_CONFIGURATION_PARAMETER_DOUBLE("DoubleParameter", "Now a double", 25.3);

   // Enum type, first declare keys labels
   static const CHardwarePluginConfigurationEnumParameter<EEnumType>::ValuesNames EEnumTypeNames = boost::assign::map_list_of
      (kEnumValue1, "EnumValue1")
      (kEnumValue2, "EnumValue2")
      (kEnumValue3, "EnumValue3");
   ADD_CONFIGURATION_PARAMETER_ENUM(EEnumType, "EnumParameter", "Example of an enum with 3 values", kEnumValue2, EEnumTypeNames);

   // Special types
   // - Serial port : like enum but the available values are populated when user asks for configuration diplay
   ADD_CONFIGURATION_PARAMETER_SERIAL_PORT("Serial port", "Specific serial port parameters. Yadoms will populate available values with system serial ports.", "tty0");

   // Bits field type (= displayed as a list of checkboxes)
   static const CHardwarePluginConfigurationBitsFieldParameter::Items BitsFieldItems = boost::assign::map_list_of
      ("first checkbox", false)
      ("second one"    , true )
      ("and a third"   , true );
   ADD_CONFIGURATION_PARAMETER_BITS_FIELD("BitsFieldParameter", "Example of a bits field", BitsFieldItems);
}


CFakePlugin::CFakePlugin() 
{
}

CFakePlugin::~CFakePlugin()
{
}

void CFakePlugin::onMessageReceived(CXplMessage & message)
{
   // Be carefull, this function is called in the Xpl service thread context
   //YADOMS_LOG(info) << "Message received : " << message.toString();
}

void CFakePlugin::doWork(const std::string& configurationValues)
{
   //TODO : understand why it throws when this step is done in xplService dtor, which is called when catching boost::thread_interrupted
   boost::shared_ptr<CXplService> xplService;
   try
   {
      YADOMS_LOG_CONFIGURE("FakePlugin");
      xplService.reset(new CXplService("yadoms", "fake", "1"));
      xplService->messageReceived(boost::bind(&CFakePlugin::onMessageReceived, this, _1));

      // Build configuration and load values from database
      CHardwarePluginConfiguration configuration(getConfigurationSchema(), configurationValues);
      traceConfiguration(configuration);

      int value = 0;

      while(1)
      {
         YADOMS_LOG(debug) << "CFakePlugin is running...";

         std::string newConfigurationValues = getUpdatedConfiguration();
         if (!newConfigurationValues.empty())
         {
            // Take here into account the new configuration
            // - Restart the plugin if necessary,
            // - Update some ressources,
            // - etc...
            configuration.setValues(newConfigurationValues);
            YADOMS_LOG(debug) << "CFakePlugin configuration was updated...";
            traceConfiguration(configuration);
         }

         CXplMessage msg(CXplMessage::kXplStat, xplService->getActor(), CXplActor::createBroadcastActor(), CXplMessageSchemaIdentifier("clock", "basic"));
         msg.addToBody("value", boost::lexical_cast<std::string>(value++));
         xplService->sendMessage(msg);

         // Give a chance to exit plugin thread
         boost::this_thread::sleep(boost::posix_time::milliseconds(1000)); 
      };
   }
   // Plugin must catch this end-of-thread exception to make its cleanup.
   // If no cleanup is necessary, still catch it, or Yadoms will consider
   // as a plugin failure.
   catch (boost::thread_interrupted&)
   {
      YADOMS_LOG(info) << "CFakePlugin is stopping..."  << std::endl;
   }
   catch(...)
   {
   }

   //TODO : understand why it throws when this step is done in xplService dtor, which is called when catching boost::thread_interrupted
   try
   {
      if(xplService.get() != NULL)
         xplService->stop();
   }
   catch(...)
   {
   }
}

void CFakePlugin::updateConfiguration(const std::string& configurationValues)
{
   boost::lock_guard<boost::mutex> lock(m_configurationUpdateMutex);
   m_ConfigurationUpdateQueue.push(configurationValues);
}

std::string CFakePlugin::getUpdatedConfiguration() // TODO à mettre dans IMPLEMENT_CONFIGURATION (ou class CHardwarePluginConfigurationSupport)
{
   boost::lock_guard<boost::mutex> l(m_configurationUpdateMutex);

   if (m_ConfigurationUpdateQueue.empty())
      return std::string();

   std::string newConfiguration = m_ConfigurationUpdateQueue.back();
   m_ConfigurationUpdateQueue.pop();
   return newConfiguration;
}

void CFakePlugin::traceConfiguration(const CHardwarePluginConfiguration& configuration)
{
   try
   {
      // Read parameter as string
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'Serial port' is " << configuration["Serial port"].valueToString();
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'StringParameter' is " << configuration["StringParameter"].valueToString();
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BoolParameter' is " << configuration["BoolParameter"].valueToString();
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'EnumParameter' is " << configuration["EnumParameter"].valueToString();

      // More usable form : read parameter as its basic type
      if (configuration.asBool("BoolParameter"))
         YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BoolParameter' is true";
      else
         YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BoolParameter' is false";
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'StringParameter' is " << configuration.asString("StringParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'Serial port' is " << configuration.asSerialPort("Serial port");
      std::ostringstream os;
      os << "CFakePlugin::doWork, parameter 'EnumParameter' is ";
      switch (configuration.asEnum<EEnumType>("EnumParameter"))
      {
      case kEnumValue1: os << "EnumValue1"; break;
      case kEnumValue2: os << "EnumValue2"; break;
      case kEnumValue3: os << "EnumValue3"; break;
      default: os << "Invalid value"; break;
      }
      YADOMS_LOG(debug) << os.str();

      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BitsFieldParameter[first checkbox]' is " << (configuration.asBitsField("BitsFieldParameter")["first checkbox"] ? "true" : "false");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BitsFieldParameter[second one]' is " << (configuration.asBitsField("BitsFieldParameter")["second one"] ? "true" : "false");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BitsFieldParameter[and a third]' is " << (configuration.asBitsField("BitsFieldParameter")["and a third"] ? "true" : "false");
   }
   catch (const CBadConversionException& bc)
   {
      BOOST_ASSERT(false);  // Parameter is wrong type
      YADOMS_LOG(error) << "Bad cast error: " << bc.what();
   }
   catch (const std::out_of_range& oor)
   {
      BOOST_ASSERT(false);  // Parameter doesn't exist
      YADOMS_LOG(error) << "Out of Range error: " << oor.what();
   }
}


// TODO : WhatTheFuck ? ? ? C'est quoi ces adresses ?
void CFakePlugin::init(const std::string & pluginAddress, const std::string & serverAddress)
{
}


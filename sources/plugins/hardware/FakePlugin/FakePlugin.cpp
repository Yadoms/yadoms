#include "stdafx.h"
#include "FakePlugin.h"
#include <shared/Log.h>
#include <shared/Xpl/XplService.h>
#include <shared/Xpl/XplMessage.h>
#include <shared/Exceptions/BadConversionException.hpp>
#include <shared/StringExtension.h>

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


// If parameters are needed for the plugin, just use the IMPLEMENT_CONFIGURATION macro
// to create the configuration schema and add parameters
//TODO remettre la macro IMPLEMENT_CONFIGURATION
const CFakePluginConfiguration& getConfigurationSchema()
{
   static boost::shared_ptr<CFakePluginConfiguration> Configuration;

   if (Configuration)
      return *Configuration;   /* Already initialized */
   Configuration.reset(new CFakePluginConfiguration);
   Configuration->buildSchema();
   return *Configuration;
}

EXPORT_LIBRARY_FUNCTION const IHardwarePluginConfigurationSchema& getConfigurationSchemaInterface()
{
   return getConfigurationSchema();
}


CFakePlugin::CFakePlugin()
{
   m_Configuration.buildSchema();
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
      m_Configuration.setValues(configurationValues);
      // Trace the configuration (just for test)
      traceConfiguration();

      int value = 0;

      while(1)
      {
         YADOMS_LOG(debug) << "CFakePlugin is running...";

         std::string newConfigurationValues = getUpdatedConfiguration();
         if (!newConfigurationValues.empty())
         {
            // Take into account the new configuration
            // - Restart the plugin if necessary,
            // - Update some resources,
            // - etc...
            m_Configuration.setValues(newConfigurationValues);
            YADOMS_LOG(debug) << "CFakePlugin configuration was updated...";
            // Trace the configuration (just for test)
            traceConfiguration();
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
      return CStringExtension::EmptyString;

   std::string newConfiguration = m_ConfigurationUpdateQueue.back();
   m_ConfigurationUpdateQueue.pop();
   return newConfiguration;
}

void CFakePlugin::traceConfiguration()
{
   try
   {
      // Read parameter as string
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'Serial port' is " << m_Configuration["Serial port"].valueToString();
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'StringParameter' is " << m_Configuration["StringParameter"].valueToString();
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BoolParameter' is " << m_Configuration["BoolParameter"].valueToString();
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'EnumParameter' is " << m_Configuration["EnumParameter"].valueToString();

      // More usable form : read parameter as its basic type
      if (m_Configuration.asBool("BoolParameter"))
         YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BoolParameter' is true";
      else
         YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BoolParameter' is false";
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'StringParameter' is " << m_Configuration.asString("StringParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'Serial port' is " << m_Configuration.asSerialPort("Serial port");
      std::ostringstream os;
      os << "CFakePlugin::doWork, parameter 'EnumParameter' is ";
      switch (m_Configuration.asEnum<EEnumType>("EnumParameter"))
      {
      case kEnumValue1: os << "EnumValue1"; break;
      case kEnumValue2: os << "EnumValue2"; break;
      case kEnumValue3: os << "EnumValue3"; break;
      default: os << "Invalid value"; break;
      }
      YADOMS_LOG(debug) << os.str();

      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BitsFieldParameter[first checkbox]' is " << (m_Configuration.asBitsField("BitsFieldParameter")["first checkbox"] ? "true" : "false");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BitsFieldParameter[second one]' is " << (m_Configuration.asBitsField("BitsFieldParameter")["second one"] ? "true" : "false");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BitsFieldParameter[and a third]' is " << (m_Configuration.asBitsField("BitsFieldParameter")["and a third"] ? "true" : "false");
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


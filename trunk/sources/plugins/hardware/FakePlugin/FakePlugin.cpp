#include "stdafx.h"
#include "FakePlugin.h"
#include <shared/Log.h>
#include <shared/Xpl/XplService.h>
#include <shared/Xpl/XplMessage.h>
#include <shared/Xpl/XplConstants.h>
#include <shared/Exceptions/BadConversionException.hpp>
#include <shared/StringExtension.h>


// Use this macro to define some basic informations about the plugin
IMPLEMENT_HARDWARE_PLUGIN(
   CFakePlugin,                                 // IHardwarePlugin implementation class
   "FakePlugin",                                // Plugin name (std::string)
   "0.1",                                       // Current plugin version (std::string)
   shared::plugin::information::kBeta,          // Current release state (shared::plugin::information::EReleaseType)
   "yadoms-team",                               // Author name (std::string)
   "http://sourceforge.net/projects/yadoms/")   // Url of author web site (std::string)


CFakePlugin::CFakePlugin()
{
   // Build the schema
   m_Configuration.buildSchema();
}

CFakePlugin::~CFakePlugin()
{
}

// Event IDs
enum
{
   kEvtXplMessage = CEventHandler::kUserFirstId,   // Always start from CEventHandler::kUserFirstId
   kEvtUpdateConfiguration
};

void CFakePlugin::doWork(const std::string& configurationValues, boost::asio::io_service * pluginIOService)
{
   //TODO : understand why it throws when this step is done in xplService dtor, which is called when catching boost::thread_interrupted
   
   try
   {
      YADOMS_LOG_CONFIGURE("FakePlugin");
      YADOMS_LOG(debug) << "CFakePlugin is starting...";

      CXplService xplService(CXplConstants::getYadomsVendorId(), "fake", "1", pluginIOService);
      
      //use this line to use be notified from CEventHandler on an xplMessage
      xplService.messageReceived(this, kEvtXplMessage);

      // Load configuration values (provided by database)
      m_Configuration.setValues(configurationValues);
      // Trace the configuration (just for test)
      m_Configuration.trace();

      int value = 0;

      YADOMS_LOG(debug) << "CFakePlugin is running...";
      while(1)
      {
         // Wait for an event, with timeout
         switch(waitForEvents(boost::posix_time::milliseconds(20000)))
         {
         case kEvtXplMessage:
            {
               // Xpl message was received
               CXplMessage xplMessage = popEvent<CXplMessage>();
               //YADOMS_LOG(debug) << "XPL message event received :" << xplMessage.toString();

               /*
               static int currentCall = 0;     
               currentCall++;

               if(currentCall == 100)
               {
                  YADOMS_LOG(debug) << "le plugin plante";
                  while(1);
               }*/

               break;
            }
         case kEvtUpdateConfiguration:
            {
               // Configuration was updated
               std::string newConfigurationValues = popEvent<std::string>();
               YADOMS_LOG(debug) << "configuration was updated...";
               BOOST_ASSERT(!newConfigurationValues.empty());  // newConfigurationValues shouldn't be empty, or kEvtUpdateConfiguration shouldn't be generated

               // Take into account the new configuration
               // - Restart the plugin if necessary,
               // - Update some resources,
               // - etc...
               m_Configuration.setValues(newConfigurationValues);

               // Trace the configuration
               traceConfiguration();

               break;
            }
         case kTimeout:
            {
               // Timeout, used here to send a XPL message periodicaly
               //CXplConstants::getYadomsVendorId(), "logger", "1"
               
               CXplMessage msg(CXplMessage::kXplStat, xplService.getActor(), CXplActor::createActor(CXplConstants::getYadomsVendorId(), "logger", "1"), CXplMessageSchemaIdentifier("clock", "basic"));
               //CXplMessage msg(CXplMessage::kXplStat, xplService.getActor(), CXplActor::createBroadcastActor(), CXplMessageSchemaIdentifier("clock", "basic"));
               msg.addToBody("value", boost::lexical_cast<std::string>(value++));
               xplService.sendMessage(msg);
               break;
            }
         default:
            {
               YADOMS_LOG(error) << "Unknown message id";

               // We need to consume this unknown event
               popEvent();

               break;
            }
         }
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

}

void CFakePlugin::updateConfiguration(const std::string& configurationValues)
{
   // This function is called in a Yadoms thread context, so send a event to CFakePlugin thread
   sendEvent<std::string>(kEvtUpdateConfiguration, configurationValues);
}

void CFakePlugin::traceConfiguration()
{
   try
   {
      // Get simple parameters
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'Serial port' is " << m_Configuration.getParam<shared::plugin::configuration::CSerialPortParameter>("Serial port").get();
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'Serial port' is (with macro helper) " << m_Configuration.CFG_GET_SERIAL_PORT("Serial port");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'StringParameter' is " << m_Configuration.CFG_GET_STRING("StringParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BoolParameter' is " << m_Configuration.CFG_GET_BOOL("BoolParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'DoubleParameter' is " << m_Configuration.CFG_GET_DOUBLE("DoubleParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'IntParameter' is " << m_Configuration.CFG_GET_INT("IntParameter");

      // Enum
      // - Nominal form
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'EnumParameter' is " << m_Configuration.getParam<shared::plugin::configuration::CEnumParameter<EEnumType> >("EnumParameter").get();
      // - With macro helper
      std::ostringstream os;
      os << "CFakePlugin::doWork, parameter 'EnumParameter' is ";
      switch (m_Configuration.CFG_GET_ENUM(EEnumType,"EnumParameter"))
      {
      case kEnumValue1: os << "EnumValue1"; break;
      case kEnumValue2: os << "EnumValue2"; break;
      case kEnumValue3: os << "EnumValue3"; break;
      default: os << "Invalid value"; break;
      }
      YADOMS_LOG(debug) << os.str();

      // Bits field
      // - Nominal form
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BitsFieldParameter[first checkbox]' is " << m_Configuration.getParam<shared::plugin::configuration::CBitsFieldParameter>("BitsFieldParameter").get()["first checkbox"];
      // - With macro helper
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BitsFieldParameter[second one]' is " << m_Configuration.CFG_GET_BITS_FIELD("BitsFieldParameter")["second one"];
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BitsFieldParameter[and a third]' is " << m_Configuration.CFG_GET_BITS_FIELD("BitsFieldParameter")["and a third"];
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


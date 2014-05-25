#include "stdafx.h"
#include "WindowsSystem.h"
#include <shared/Log.h>
#include <shared/xpl/XplService.h>
#include <shared/xpl/XplMessage.h>
#include <shared/xpl/XplHelper.h>
#include <shared/event/EventTimer.h>
#include "WindowsSystemMemoryLoad.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values
IMPLEMENT_PLUGIN(CWindowsSystem)


CWindowsSystem::CWindowsSystem()
{
}

CWindowsSystem::~CWindowsSystem()
{
}

// Event IDs
enum
{
   kEvtXplMessage = shared::event::kUserFirstId,   // Always start from shared::event::CEventHandler::kUserFirstId
   kEvtTimerSendMessage
};

// XPL device ID : use to identify this plugin over the XPL network.
// Must match XPL rules (see http://xplproject.org.uk/wiki/index.php/XPL_Specification_Document) :
//   - alphanumerical characters only : [0-9][a-z]
//   - letters must be lower case
//   - no characters '.', '-', '_', or so...
//   - 8 characters max
// NOTE : To avoid DeviceId conflicts, YADOMS-TEAM manage a device ID reservation table. Please contact to reserve your device ID.
// You can check that your device ID mach Xpl rules calling shared::xpl::CXplHelper::matchRules(shared::xpl::CXplHelper::kDeviceId, your_device_id)
static const std::string& XplDeviceId("winsys");

void CWindowsSystem::doWork(int instanceUniqueId, const std::string& configuration, boost::asio::io_service * pluginIOService)
{
   try
   {
      YADOMS_LOG_CONFIGURE(Informations->getName());
      YADOMS_LOG(debug) << "CWindowsSystem is starting...";


      // Register to XPL service
            // Register to XPL service
      shared::xpl::CXplService xplService(
         XplDeviceId,                                                // XPL device ID : use to identify this plugin over the XPL network
         shared::xpl::CXplHelper::toInstanceId(instanceUniqueId),    // Use the plugin instance id (guaranteed by Yadoms to be unique among all instances of all plugins) as XPL instance id
         pluginIOService);                                           // Use the provided io service for better performance
/*      m_xplService.reset(new shared::xpl::CXplService(
         XplDeviceId,
         shared::xpl::CXplHelper::toInstanceId(instanceUniqueId),
         pluginIOService));
*/
      // Fake temperature sensor
	  //TODO : Faire la charge mémoire à ce niveau
      CWindowsSystemMemoryLoad MemoryLoad("MemoryLoad");

      // Timer used to send a XPL message periodically
      createTimer(kEvtTimerSendMessage, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(30));

      // the main loop
      YADOMS_LOG(debug) << "WindowsSystem plugin is running...";

      while(1)
      {
         // Wait for an event
         switch(waitForEvents())
         {
         case kEvtTimerSendMessage:
            {
               // Timer used here to send a XPL message periodically

               YADOMS_LOG(debug) << "WindowsSystem plugin :  Read a value...";

               // First read the memory load
               if(MemoryLoad.read())
               {
				   // Create the message
				   shared::xpl::CXplMessage msg(
					  shared::xpl::CXplMessage::kXplStat,                            // Message type
					  xplService.getActor(),                                         // Source actor (here : our fake plugin instance)
					  shared::xpl::CXplActor::createBroadcastActor(),                // Target actor (here : the XPL logger of Yadoms)
					  shared::xpl::CXplMessageSchemaIdentifier("sensor", "basic"));  // The message schema

				   // Add data to message
				   // - Device ID
				   msg.addToBody("device", MemoryLoad.getDeviceId());

				   // - Memory in %
				   msg.addToBody("type", "mem");
				   msg.addToBody("units", "%");
				   std::ostringstream ss;
				   ss << std::fixed << std::setprecision(2) << MemoryLoad.getMemoryLoad();
				   msg.addToBody("current", ss.str());
				   // Send it
				   xplService.sendMessage(msg);

               YADOMS_LOG(debug) << "WindowsSystem plugin :  Send a value : " << ss.str();
               }
               else
               {
                  YADOMS_LOG(debug) << "WindowsSystem plugin :  Error Reading Value...";
               }

               break;
            }
         default:
            {
               YADOMS_LOG(error) << "Unknown message id";
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
      YADOMS_LOG(info) << "WindowsSystem is stopping..."  << std::endl;
   }
}

void CWindowsSystem::updateConfiguration(const std::string& configuration)
{

}


#include <boost/shared_ptr.h>
#include "../../../sources/server/pluginSystem/Manager.h"

#define BOOST_TEST_MODULE TestHardwarePluginManager

#include <boost/test/unit_test.hpp>


static const std::string PluginPath("testPluginPath");
static CMokePluginRequester PluginRequester;
static CMokePluginEventLoggerRequester PluginEventLoggerRequester;
static CMokeEventHandler EventHandler;
enum { kPluginManagerEvent = 0 };

//--------------------------------------------------------------
/// \brief	    Test plugin configuration getter
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestPluginConfigurationGet)
{
   boost::shared_ptr<pluginSystem::CManager> pluginManager = pluginSystem::CManager::newManager(
      PluginPath, PluginRequester, PluginEventLoggerRequester, EventHandler, kPluginManagerEvent);

}
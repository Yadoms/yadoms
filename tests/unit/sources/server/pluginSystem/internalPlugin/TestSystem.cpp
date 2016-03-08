#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/DataContainer.h"
#include "../../../../sources/shared/shared/plugin/yPluginApi/IYPluginApi.h"
#include "../../../../sources/shared/shared/event/EventHandler.hpp"
#include "../../../../sources/shared/shared/ServiceLocator.h"
#include "../../../../sources/server/automation/script/IObjectFactory.h"
#include "../../../../sources/server/pluginSystem/internalPlugin/System.h"
#include "../../../../sources/server/IApplicationStopHandler.h"

// Includes needed to compile the test
#include "../../../testCommon/fileSystem.h"

#include "../../../mock/server/pluginSystem/information/DefaultInformationMock.hpp"
#include "../../../mock/server/pluginSystem/DefaultYPluginApiMock.hpp"
#include "../../../mock/shared/currentTime/DefaultCurrentTimeMock.hpp"

BOOST_AUTO_TEST_SUITE(TestInternalPlugin)

namespace yApi = shared::plugin::yPluginApi;

class CHistorizableMock : public shared::plugin::yPluginApi::historization::IHistorizable
{
public:
   CHistorizableMock() : m_keyword(std::string()), m_capacity(std::string(), std::string(), shared::plugin::yPluginApi::EKeywordDataType::kNoDataValue),
      m_accessMode(shared::plugin::yPluginApi::EKeywordAccessMode::kNoAccessValue), m_measure_type(shared::plugin::yPluginApi::historization::EMeasureType::kAbsoluteValue){}
   virtual ~CHistorizableMock(){}

   // IHistorizable implementation
   virtual const std::string& getKeyword() const { return m_keyword; }
   virtual const shared::plugin::yPluginApi::CStandardCapacity& getCapacity() const { return m_capacity; }
   virtual const shared::plugin::yPluginApi::EKeywordAccessMode& getAccessMode() const { return m_accessMode; }
   virtual std::string formatValue() const { return std::string(); }
   virtual const shared::plugin::yPluginApi::historization::EMeasureType& getMeasureType() const { return m_measure_type; }
   virtual shared::CDataContainer getTypeInfo() const { return shared::CDataContainer(); }
   // [END] IHistorizable implementation

private:
   const std::string m_keyword;
   const shared::plugin::yPluginApi::CStandardCapacity m_capacity;
   const shared::plugin::yPluginApi::EKeywordAccessMode m_accessMode;
   const shared::plugin::yPluginApi::historization::EMeasureType m_measure_type;
};

class CCommandMock : public yApi::IDeviceCommand
{
public:
   CCommandMock(const std::string& device, const std::string& keyword, const std::string& body = std::string()) : m_device(device), m_keyword(keyword), m_body(body){}
   virtual ~CCommandMock(){}

   // yApi::IDeviceCommand implementation
   virtual const std::string& getTargetDevice() const { return m_device; }
   virtual const std::string& getKeyword() const { return m_keyword; }
   virtual const std::string& getBody() const { return m_body; }
   virtual const std::string toString() const { return std::string(); }
   virtual const shared::plugin::yPluginApi::historization::IHistorizable& getHistorizableObject() const { return m_historizable; }
   // [END] yApi::IDeviceCommand implementation

private:
   const std::string m_device;
   const std::string m_keyword;
   const std::string m_body;
   const CHistorizableMock m_historizable;
};

class CStopHandlerMock : public IApplicationStopHandler
{
public:
   CStopHandlerMock():m_lastStopRequest(kYadomsOnly){}
   virtual ~CStopHandlerMock(){}

   // IApplicationStopHandler implementation
   virtual void requestToStop(EStopMode stopMode) { m_evtHandler.postEvent(shared::event::kUserFirstId, stopMode); }
   virtual void registerForAppEnds(boost::shared_ptr<shared::event::CEventHandler> & handler, const int code) {};
   // [END] IApplicationStopHandler implementation

   EStopMode waitStopRequest()
   {
      BOOST_CHECK_EQUAL(m_evtHandler.waitForEvents(boost::posix_time::seconds(3)), shared::event::kUserFirstId);
      return m_evtHandler.getEventData<EStopMode>();
   }

private:
   shared::event::CEventHandler m_evtHandler;
   EStopMode m_lastStopRequest;
};

class DayLightMock : public automation::script::IDayLight
{
public:
   ~DayLightMock(){}
   virtual boost::posix_time::ptime sunriseTime() { return boost::posix_time::ptime(shared::currentTime::Provider::now().date(), boost::posix_time::time_duration(7, 30, 0)); }
   virtual boost::posix_time::ptime sunsetTime() { return boost::posix_time::ptime(shared::currentTime::Provider::now().date(), boost::posix_time::time_duration(19, 15, 0)); }
};

class LocationMock : public automation::script::ILocation
{
public:
   ~LocationMock(){}
   virtual double latitude() const { return 44.9232911; }
   virtual double longitude() const { return 4.916384; }
   virtual double altitude() const { return 169.0; }
};

class AutomationScriptObjectFactoryMock : public automation::script::IObjectFactory
{
public:
   AutomationScriptObjectFactoryMock(boost::shared_ptr<automation::script::IDayLight> dayLight, boost::shared_ptr<automation::script::ILocation> location) :m_dayLight(dayLight), m_location(location){}
   ~AutomationScriptObjectFactoryMock(){}
   virtual boost::shared_ptr<automation::script::IAutoLocation> getAutoLocationService() { return boost::shared_ptr<automation::script::IAutoLocation>(); }
   virtual boost::shared_ptr<automation::script::ILocation> getLocation() { return m_location; }
   virtual boost::shared_ptr<automation::script::IDayLight> getDayLight() { return m_dayLight; }
private:
   boost::shared_ptr<automation::script::IDayLight> m_dayLight;
   boost::shared_ptr<automation::script::ILocation> m_location;
};

void ThreadFunction(pluginSystem::internalPlugin::CSystem& systemPlugin, boost::shared_ptr<yApi::IYPluginApi> context)
{
   systemPlugin.doWork(context);
}

BOOST_AUTO_TEST_CASE(Nominal)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   pluginSystem::internalPlugin::CSystem systemPlugin;
   boost::shared_ptr<CDefaultYPluginApiMock> context(boost::make_shared<CDefaultYPluginApiMock>());
   boost::shared_ptr<CStopHandlerMock> stopHandler(boost::make_shared<CStopHandlerMock>());
   shared::CServiceLocator::instance().push<IApplicationStopHandler>(stopHandler);
   boost::shared_ptr<automation::script::ILocation> locationProvider(boost::make_shared<LocationMock>());
   boost::shared_ptr<automation::script::IDayLight> dayLightProvider(boost::make_shared<DayLightMock>());
   boost::shared_ptr<automation::script::IObjectFactory> automationScriptObjectFactoryMock(boost::make_shared<AutomationScriptObjectFactoryMock>(dayLightProvider, locationProvider));
   shared::CServiceLocator::instance().push<automation::script::IObjectFactory>(automationScriptObjectFactoryMock);
   
   boost::thread systemPluginThread(ThreadFunction, systemPlugin, context);
   
   boost::this_thread::sleep(boost::posix_time::milliseconds(100));

   // Check devices/keywords declared by systemPlugin
   BOOST_CHECK(context->getDevices().size() == 1);
   BOOST_CHECK(context->deviceExists("system"));
   BOOST_CHECK(context->getKeywords().size() == 2);
   BOOST_CHECK(context->keywordExists("system", "shutdown"));
   BOOST_CHECK(context->keywordExists("system", "restart"));

   // Check commands
   boost::shared_ptr<const yApi::IDeviceCommand> shutdownCommand(boost::make_shared<CCommandMock>("system", "shutdown"));
   context->getEventHandler().postEvent(yApi::IYPluginApi::kEventDeviceCommand, shutdownCommand);
   BOOST_CHECK_EQUAL(stopHandler->waitStopRequest(), IApplicationStopHandler::kStopSystem);

   boost::shared_ptr<const yApi::IDeviceCommand> restartCommand(boost::make_shared<CCommandMock>("system", "restart"));
   context->getEventHandler().postEvent(yApi::IYPluginApi::kEventDeviceCommand, restartCommand);
   BOOST_CHECK_EQUAL(stopHandler->waitStopRequest(), IApplicationStopHandler::kRestartSystem);

   systemPluginThread.interrupt();
   BOOST_CHECK_EQUAL(systemPluginThread.timed_join(boost::posix_time::milliseconds(100)), true);
}

BOOST_AUTO_TEST_SUITE_END()
#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/DataContainer.h"
#include "../../../../sources/shared/shared/plugin/yPluginApi/IYPluginApi.h"
#include "../../../../sources/shared/shared/event/EventHandler.hpp"
#include "../../../../sources/shared/shared/ServiceLocator.h"
#include "../../../../sources/server/pluginSystem/internalPlugin/System.h"
#include "../../../../sources/server/IApplicationStopHandler.h"

// Includes needed to compile the test
#include "../../../testCommon/fileSystem.h"

#include "../../../mock/server/pluginSystem/information/DefaultInformationMock.hpp"
#include "../../../mock/server/pluginSystem/DefaultYPluginApiMock.hpp"

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
   virtual void requestToStop(EStopMode stopMode) { m_lastStopRequest=stopMode; }
   virtual void registerForAppEnds(boost::shared_ptr<shared::event::CEventHandler> & handler, const int code) {};
   // [END] IApplicationStopHandler implementation

   EStopMode getLastStopRequest() { return m_lastStopRequest; }

private:
   EStopMode m_lastStopRequest;
};

void ThreadFunction(pluginSystem::internalPlugin::CSystem& systemPlugin, boost::shared_ptr<yApi::IYPluginApi> context)
{
   systemPlugin.doWork(context);
}

BOOST_AUTO_TEST_CASE(Nominal)
{
   pluginSystem::internalPlugin::CSystem systemPlugin;
   boost::shared_ptr<CDefaultYPluginApiMock> context(new CDefaultYPluginApiMock());
   boost::shared_ptr<CStopHandlerMock> stopHandler(new CStopHandlerMock());
   shared::CServiceLocator::instance().push<IApplicationStopHandler>(stopHandler);
   
   boost::thread systemPluginThread(ThreadFunction, systemPlugin, context);
   
   boost::this_thread::sleep(boost::posix_time::milliseconds(100));

   // Check devices/keywords declared by systemPlugin
   BOOST_CHECK(context->getDevices().size() == 1);
   BOOST_CHECK(context->deviceExists("system"));
   BOOST_CHECK(context->getKeywords().size() == 2);
   BOOST_CHECK(context->keywordExists("system", "shutdown"));
   BOOST_CHECK(context->keywordExists("system", "restart"));

   // Check commands
   boost::shared_ptr<const yApi::IDeviceCommand> shutdownCommand(new CCommandMock("system", "shutdown"));
   context->getEventHandler().postEvent(yApi::IYPluginApi::kEventDeviceCommand, shutdownCommand);
   boost::this_thread::sleep(boost::posix_time::milliseconds(100));
   BOOST_CHECK_EQUAL(stopHandler->getLastStopRequest(), IApplicationStopHandler::kStopSystem);

   boost::shared_ptr<const yApi::IDeviceCommand> restartCommand(new CCommandMock("system", "restart"));
   context->getEventHandler().postEvent(yApi::IYPluginApi::kEventDeviceCommand, restartCommand);
   boost::this_thread::sleep(boost::posix_time::milliseconds(100));
   BOOST_CHECK_EQUAL(stopHandler->getLastStopRequest(), IApplicationStopHandler::kRestartSystem);

   systemPluginThread.interrupt();
   BOOST_CHECK_EQUAL(systemPluginThread.timed_join(boost::posix_time::milliseconds(100)), true);
}

BOOST_AUTO_TEST_SUITE_END()
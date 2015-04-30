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

BOOST_AUTO_TEST_SUITE(TestInternalPlugin)

// Moke IInformation
class CDefaultInformationMock : public shared::plugin::information::IInformation
{
public:
   virtual ~CDefaultInformationMock() {}
   // shared::plugin::IInformation implementation
   virtual const std::string& getName() const { static const std::string str("mock"); return str; }
   virtual const std::string& getDescription() const { static const std::string str("mockDescription"); return str; }
   virtual const std::string& getVersion() const { static const std::string str("99"); return str; }
   virtual const shared::plugin::information::EReleaseType getReleaseType() const { return shared::plugin::information::kBeta; }
   virtual const std::string& getAuthor() const { static const std::string str("DarkVador"); return str; }
   virtual const std::string& getUrl() const { static const std::string str("www.somewhere.inthegalaxy"); return str; }
   virtual std::string getIdentity() const { std::ostringstream formatedInformations; formatedInformations << getName() << " v" << getVersion() << "[" << boost::lexical_cast<std::string>(getReleaseType()) << "]"; return formatedInformations.str(); }
   virtual std::string toString() const { std::ostringstream formatedInformations; formatedInformations << getIdentity() << " by " << getAuthor() << " (" << getUrl() << ")"; return formatedInformations.str(); }
   virtual bool getSupportManuallyCreatedDevice() const { return false; }
   virtual bool isSupportedOnThisPlatform() const { return true; }
   // [END] shared::plugin::IInformation implementation
};

// Moke IYPluginApi
namespace yApi = shared::plugin::yPluginApi;
class CDefaultYPluginApiMock : public yApi::IYPluginApi
{
public:
   struct Device { std::string m_model; shared::CDataContainer m_details; };
   struct Keyword { std::string m_device; std::string m_keyword; std::string m_capacity; shared::CDataContainer m_details; };
   struct Data { std::string m_device; std::string m_keyword; std::string m_capacity; std::string m_value; };
public:
   virtual ~CDefaultYPluginApiMock() {}
   // IYPluginApi implementation
   virtual shared::event::CEventHandler & getEventHandler() { return m_defaultEventHandler; }
   virtual bool deviceExists(const std::string& device) const { return m_devices.find(device) != m_devices.end(); }
   virtual const shared::CDataContainer getDeviceDetails(const std::string& device) const { return m_devices.find(device)->second.m_details; }
   virtual void declareDevice(const std::string& device, const std::string& model, const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer)
   { Device dev = {model, details}; m_devices[device] = dev; }
   virtual bool keywordExists(const std::string& device, const std::string& keyword) const { return m_keywords.find(keyword) != m_keywords.end(); }
   virtual bool keywordExists(const std::string& device, const yApi::historization::IHistorizable& keyword) const { return false; }
   virtual void declareKeyword(const std::string& device, const yApi::historization::IHistorizable& keyword, const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer)
   { Keyword kw = { device, keyword.getKeyword(), keyword.getCapacity(), details }; m_keywords[keyword.getKeyword()] = kw; }
   virtual std::string getRecipientValue(int recipientId, const std::string& fieldName) const { return std::string(); }
   virtual std::vector<int> findRecipientsFromField(const std::string& fieldName, const std::string& expectedFieldValue) const { return std::vector<int>(); }
   virtual bool recipientFieldExists(const std::string& fieldName) const { return false; }
   virtual void historizeData(const std::string& device, const yApi::historization::IHistorizable& keyword)
   { Data data = {device, keyword.getKeyword(), keyword.getCapacity(), keyword.formatValue() }; m_data.push_back(data); }
   virtual void historizeData(const std::string& device, std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > & dataVect) {  /* Not implemented */ }
   virtual const shared::plugin::information::IInformation& getInformation() const { return m_defaultInformation; }
   virtual const boost::filesystem::path getPluginPath() const { return boost::filesystem::initial_path<boost::filesystem::path>(); }
   virtual shared::CDataContainer getConfiguration() const { return m_defaultConfiguration; }
   virtual void recordPluginEvent(PluginEventSeverity severity, const std::string & message) {}
   // [END] IYPluginApi implementation

   const std::map<std::string, Device>& getDevices() const { return m_devices; }
   const std::map<std::string, Keyword>& getKeywords() const { return m_keywords; }
   const std::vector<Data>& getData() const { return m_data; }
protected:
   shared::event::CEventHandler m_defaultEventHandler;
   CDefaultInformationMock m_defaultInformation;
   shared::CDataContainer m_defaultConfiguration;
   std::map<std::string, Device> m_devices;
   std::map<std::string, Keyword> m_keywords;
   std::vector<Data> m_data;
};

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
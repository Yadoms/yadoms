#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include <../../../../sources/shared/shared/plugin/yadomsApi/StandardCapacities.h>
#include "../../../../sources/plugins/FakePlugin/FakeSensor.h"

BOOST_AUTO_TEST_SUITE(TestFakeSensor)

static const std::string sensorId("MySensor");

// Moke IInformation //TODO � exporter dans un autre fichier
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
   virtual std::string toString() const { std::ostringstream formatedInformations; formatedInformations << getIdentity() << " by " << getAuthor() << " (" << getUrl() << ")"; return formatedInformations.str(); }
   virtual std::string getIdentity() const { std::ostringstream formatedInformations; formatedInformations << getName() << " v" << getVersion() << "[" << boost::lexical_cast<std::string>(getReleaseType()) << "]"; return formatedInformations.str(); }
   // [END] shared::plugin::IInformation implementation
};

// Moke IYadomsApi //TODO � exporter dans un autre fichier
namespace yApi = shared::plugin::yadomsApi;
class CDefaultYadomsApiMock : public yApi::IYadomsApi
{
public:
   struct Device { std::string m_model; std::string m_details; };
   struct Keyword { std::string m_device; std::string m_capacity; EKeywordAccessMode m_accessMode; EKeywordType m_type; std::string m_units; shared::CDataContainer m_details; };
public:
   virtual ~CDefaultYadomsApiMock() {}
   // IYadomsApi implementation
   virtual shared::event::CEventHandler & getEventHandler() { return m_defaultEventHandler; }
   virtual bool deviceExists(const std::string& device) const { return m_devices.find(device) != m_devices.end(); }
   virtual const std::string getDeviceDetails(const std::string& device) const { return m_devices.find(device)->second.m_details; }
   virtual bool declareDevice(const std::string& device, const std::string& model, const std::string & details) { Device dev = {model, details}; m_devices[device] = dev; return true; }
   virtual bool keywordExists(const std::string& device, const std::string& keyword) const { return false; }
   virtual bool declareKeyword(const std::string& device, const std::string& keyword, const std::string& capacity, EKeywordAccessMode accessMode, EKeywordType type, const std::string & units = shared::CStringExtension::EmptyString, const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer)
   { Keyword kw = { device, capacity, accessMode, type, units, details }; m_keywords[keyword] = kw; return true; }
   virtual void historizeData(const std::string & device, const std::string & keyword, const std::string & value) {}
   virtual void historizeData(const std::string & device, const std::string & keyword, bool value) {}
   virtual void historizeData(const std::string & device, const std::string & keyword, int value) {}
   virtual void historizeData(const std::string & device, const std::string & keyword, double value) {}
   virtual const shared::plugin::information::IInformation& getInformation() const { return m_defaultInformation; }
   virtual shared::CDataContainer getConfiguration() const { return m_defaultConfiguration; }
   virtual void recordPluginEvent(PluginEventSeverity severity, const std::string & message) {}
   // [END] IYadomsApi implementation

   std::map<std::string, Device> getDevices() const { return m_devices; }
   std::map<std::string, Keyword> getKeywords() const { return m_keywords; }
protected:
   shared::event::CEventHandler m_defaultEventHandler;
   CDefaultInformationMock m_defaultInformation;
   shared::CDataContainer m_defaultConfiguration;
   std::map<std::string, Device> m_devices;
   std::map<std::string, Keyword> m_keywords;
};

BOOST_AUTO_TEST_CASE(DeviceDeclaration)
{
   CFakeSensor sensor(sensorId);
   boost::shared_ptr<CDefaultYadomsApiMock> context(new CDefaultYadomsApiMock);

   sensor.declareDevice(context);

   // Check device declaration
   BOOST_CHECK_EQUAL(context->deviceExists(sensorId), true);
   BOOST_CHECK_EQUAL(context->getDevices().size(), (unsigned int)1);

   // Check keywords declaration
   BOOST_CHECK_EQUAL(context->getKeywords().size(), (unsigned int)4);
   std::map<std::string, CDefaultYadomsApiMock::Keyword>::const_iterator itKw = context->getKeywords().find("temp1");
   if (itKw == context->getKeywords().end())
      BOOST_ERROR("temp1 keyword not found");

   CDefaultYadomsApiMock::Keyword kw = itKw->second;

   BOOST_CHECK_EQUAL(kw.m_device, sensorId);
   BOOST_CHECK_EQUAL(kw.m_capacity, yApi::CStandardCapacities::Temperature);
   BOOST_CHECK_EQUAL(kw.m_accessMode, yApi::IYadomsApi::kReadOnly);
   BOOST_CHECK_EQUAL(kw.m_type, yApi::IYadomsApi::kDecimal);
   BOOST_CHECK_EQUAL(kw.m_units, shared::CStringExtension::EmptyString);
   //BOOST_CHECK_EQUAL(kw.m_details, shared::CDataContainer::EmptyContainer);

   //TODO tester les autres KW
}

//TODO remettre en place ces tests
//BOOST_AUTO_TEST_CASE(DefaultValues)
//{
//   CFakeSensor sensor(sensorId);
//
//   BOOST_CHECK_EQUAL(sensor.getDeviceId(), sensorId);
//   BOOST_CHECK_EQUAL(sensor.getTemperature(), (double)25.0);
//   BOOST_CHECK_EQUAL(sensor.getBatteryLevel(), (int)100);
//   BOOST_CHECK_EQUAL(sensor.getRssi(), (int)50);
//}
//
//BOOST_AUTO_TEST_CASE(FirstRead)
//{
//   CFakeSensor sensor(sensorId);
//
//   BOOST_CHECK_EQUAL(sensor.getDeviceId(), sensorId);
//   BOOST_CHECK_EQUAL(sensor.getTemperature(), (double)25.0);
//   BOOST_CHECK_EQUAL(sensor.getBatteryLevel(), (int)100);
//   BOOST_CHECK_EQUAL(sensor.getRssi(), (int)50);
//
//   sensor.read();
//
//   BOOST_CHECK_GE(sensor.getTemperature(), (double)24.0);
//   BOOST_CHECK_LE(sensor.getTemperature(), (double)26.0);
//   BOOST_CHECK_EQUAL(sensor.getBatteryLevel(), (int)99);
//   BOOST_CHECK_EQUAL(sensor.getRssi(), (int)50);
//}
//
//BOOST_AUTO_TEST_CASE(BatteryDecrease)
//{
//   CFakeSensor sensor(sensorId);
//
//   // Decrease from 100 at each read
//   // Stop decrease at 20%
//   for (int i = 100 ; i >= 0 ; --i)
//   {
//      if (i >=20 )
//         BOOST_CHECK_EQUAL(sensor.getBatteryLevel(), (int)i);
//      else
//         BOOST_CHECK_EQUAL(sensor.getBatteryLevel(), (int)20);
//      sensor.read();
//   }
//}
//
//BOOST_AUTO_TEST_CASE(TemperatureVariations)
//{
//   CFakeSensor sensor(sensorId);
//
//   // Loop of 100 tests
//   // Temperature variation must be +/- 1� from previous value
//   // First value is 25�
//   double temperature = 25.0;
//   for (int i = 100 ; i >= 0 ; --i)
//   {
//      sensor.read();
//      BOOST_CHECK_GE(sensor.getTemperature(), (double)temperature - 1.0);
//      BOOST_CHECK_LE(sensor.getTemperature(), (double)temperature + 1.0);
//      temperature = sensor.getTemperature();
//   }
//}

BOOST_AUTO_TEST_SUITE_END()
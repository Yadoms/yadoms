#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include <../../../../sources/shared/shared/plugin/yadomsApi/StandardCapacities.h>
#include "../../../../sources/plugins/FakePlugin/FakeSensor.h"

BOOST_AUTO_TEST_SUITE(TestFakeSensor)

static const std::string sensorId("MySensor");

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
   virtual std::string toString() const { std::ostringstream formatedInformations; formatedInformations << getIdentity() << " by " << getAuthor() << " (" << getUrl() << ")"; return formatedInformations.str(); }
   virtual std::string getIdentity() const { std::ostringstream formatedInformations; formatedInformations << getName() << " v" << getVersion() << "[" << boost::lexical_cast<std::string>(getReleaseType()) << "]"; return formatedInformations.str(); }
   // [END] shared::plugin::IInformation implementation
};

// Moke IYadomsApi
namespace yApi = shared::plugin::yadomsApi;
class CDefaultYadomsApiMock : public yApi::IYadomsApi
{
public:
   struct Device { std::string m_model; std::string m_details; };
   struct Keyword { std::string m_device; std::string m_capacity; yApi::EKeywordAccessMode m_accessMode; yApi::EKeywordType m_type; std::string m_units; shared::CDataContainer m_details; };
   struct Data { std::string m_device; std::string m_keyword; std::string m_value; };
public:
   virtual ~CDefaultYadomsApiMock() {}
   // IYadomsApi implementation
   virtual shared::event::CEventHandler & getEventHandler() { return m_defaultEventHandler; }
   virtual bool deviceExists(const std::string& device) const { return m_devices.find(device) != m_devices.end(); }
   virtual const shared::CDataContainer getDeviceDetails(const std::string& device) const { return m_devices.find(device)->second.m_details; }
   virtual bool declareDevice(const std::string& device, const std::string& model, const std::string & details) { Device dev = {model, details}; m_devices[device] = dev; return true; }
   virtual bool keywordExists(const std::string& device, const std::string& keyword) const { return false; }
   virtual bool declareCustomKeyword(const std::string& device, const std::string& keyword, const std::string& capacity, yApi::EKeywordAccessMode accessMode, yApi::EKeywordType type, const std::string & units = shared::CStringExtension::EmptyString, const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer)
   { Keyword kw = { device, capacity, accessMode, type, units, details }; m_keywords[keyword] = kw; return true; }
   virtual bool declareKeyword(const std::string& device, const std::string& keyword, const yApi::CStandardCapacity & capacity, const shared::CDataContainer& details)
   { Keyword kw = { device, capacity.getName(), capacity.getAccessMode(), capacity.getType(), capacity.getUnit(), details }; m_keywords[keyword] = kw; return true; }
   virtual void historizeData(const std::string & device, const std::string & keyword, const std::string & value) { Data data = {device, keyword, value}; m_data.push_back(data); }
   virtual void historizeData(const std::string & device, const std::string & keyword, bool value) { Data data = {device, keyword, boost::lexical_cast<std::string>(value)}; m_data.push_back(data); }
   virtual void historizeData(const std::string & device, const std::string & keyword, int value) { Data data = {device, keyword, boost::lexical_cast<std::string>(value)}; m_data.push_back(data); }
   virtual void historizeData(const std::string & device, const std::string & keyword, double value) { Data data = {device, keyword, boost::lexical_cast<std::string>(value)}; m_data.push_back(data); }
   virtual const shared::plugin::information::IInformation& getInformation() const { return m_defaultInformation; }
   virtual shared::CDataContainer getConfiguration() const { return m_defaultConfiguration; }
   virtual void recordPluginEvent(PluginEventSeverity severity, const std::string & message) {}
   // [END] IYadomsApi implementation

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

void ckeckKeyword(boost::shared_ptr<CDefaultYadomsApiMock> context, const std::string& keyword, const std::string& device, const std::string& capacity, yApi::EKeywordAccessMode accessMode,
   yApi::EKeywordType type, const std::string& units)
{
   std::map<std::string, CDefaultYadomsApiMock::Keyword>::const_iterator itKw = context->getKeywords().find(keyword);
   if (itKw == context->getKeywords().end())
      BOOST_ERROR(keyword + "keyword not found");

   CDefaultYadomsApiMock::Keyword kw = itKw->second;

   BOOST_CHECK_EQUAL(kw.m_device, device);
   BOOST_CHECK_EQUAL(kw.m_capacity, capacity);
   BOOST_CHECK_EQUAL(kw.m_accessMode, accessMode);
   BOOST_CHECK_EQUAL(kw.m_type, type);
   BOOST_CHECK_EQUAL(kw.m_units, units);
   BOOST_CHECK_EQUAL(kw.m_details.empty(), true);
}

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
   ckeckKeyword(context, "temp1", sensorId, yApi::CStandardCapacities::Temperature, yApi::kReadOnly, yApi::kDecimal, shared::CStringExtension::EmptyString);
   ckeckKeyword(context, "temp2", sensorId, yApi::CStandardCapacities::Temperature, yApi::kReadOnly, yApi::kDecimal, shared::CStringExtension::EmptyString);
   ckeckKeyword(context, "battery", sensorId, yApi::CStandardCapacities::BatteryLevel, yApi::kReadOnly, yApi::kDecimal, shared::CStringExtension::EmptyString);
   ckeckKeyword(context, "rssi", sensorId, yApi::CStandardCapacities::Rssi, yApi::kReadOnly, yApi::kDecimal, shared::CStringExtension::EmptyString);
}

const CDefaultYadomsApiMock::Data& readLastData(boost::shared_ptr<CDefaultYadomsApiMock> context, const std::string& keyword)
{
   static CDefaultYadomsApiMock::Data noData;
   if (context->getData().empty())
   {
      BOOST_FAIL("data not found");
      return noData;
   }

   for (std::vector<CDefaultYadomsApiMock::Data>::const_reverse_iterator itData = context->getData().rbegin() ; itData != context->getData().rend() ; ++itData)
   {
      if (itData->m_keyword == keyword)
         return *itData;
   }

   BOOST_FAIL("data not found");
   return noData;  // Just to avoid compiler warning. No importance what is returned here, as test is already KO.
}

BOOST_AUTO_TEST_CASE(Historization)
{
   CFakeSensor sensor(sensorId);
   boost::shared_ptr<CDefaultYadomsApiMock> context(new CDefaultYadomsApiMock);

   sensor.historizeData(context);

   BOOST_CHECK_EQUAL(context->getData().size(), (unsigned int)4);
   BOOST_CHECK_EQUAL(readLastData(context, "temp1").m_device, sensorId);
   BOOST_CHECK_EQUAL(boost::lexical_cast<double>(readLastData(context, "temp1").m_value), 25.0);
   BOOST_CHECK_EQUAL(readLastData(context, "temp2").m_device, sensorId);
   BOOST_CHECK_EQUAL(boost::lexical_cast<double>(readLastData(context, "temp2").m_value), 10.0);
   BOOST_CHECK_EQUAL(readLastData(context, "battery").m_device, sensorId);
   BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(context, "battery").m_value), 100);
   BOOST_CHECK_EQUAL(readLastData(context, "rssi").m_device, sensorId);
   BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(context, "rssi").m_value), 50);

   sensor.read();
   sensor.historizeData(context);

   BOOST_CHECK_EQUAL(context->getData().size(), (unsigned int)8);
   BOOST_CHECK_EQUAL(readLastData(context, "temp1").m_device, sensorId);
   BOOST_CHECK_GE(boost::lexical_cast<double>(readLastData(context, "temp1").m_value), 24.0);
   BOOST_CHECK_LE(boost::lexical_cast<double>(readLastData(context, "temp1").m_value), 26.0);
   BOOST_CHECK_EQUAL(readLastData(context, "temp2").m_device, sensorId);
   BOOST_CHECK_GE(boost::lexical_cast<double>(readLastData(context, "temp2").m_value), 9.0);
   BOOST_CHECK_LE(boost::lexical_cast<double>(readLastData(context, "temp2").m_value), 11.0);
   BOOST_CHECK_EQUAL(readLastData(context, "battery").m_device, sensorId);
   BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(context, "battery").m_value), 99);
   BOOST_CHECK_EQUAL(readLastData(context, "rssi").m_device, sensorId);
   BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(context, "rssi").m_value), 50);
}

BOOST_AUTO_TEST_CASE(BatteryDecrease)
{
   CFakeSensor sensor(sensorId);
   boost::shared_ptr<CDefaultYadomsApiMock> context(new CDefaultYadomsApiMock);

   // Decrease from 100 at each read
   // Stop decrease at 20%
   for (int i = 100 ; i >= 0 ; --i)
   {
      sensor.historizeData(context);
      if (i >=20 )
         BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(context, "battery").m_value), (int)i);
      else
         BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(context, "battery").m_value), (int)20);
      sensor.read();
   }
}

BOOST_AUTO_TEST_CASE(TemperatureVariations)
{
   CFakeSensor sensor(sensorId);
   boost::shared_ptr<CDefaultYadomsApiMock> context(new CDefaultYadomsApiMock);

   // Loop of 100 tests
   // Temperature variation must be +/- 1° from previous value
   // First value is 25° for temp1, 10° for temp2
   double temp1 = 25.0, temp2 = 10.0;
   for (int i = 100 ; i >= 0 ; --i)
   {
      sensor.historizeData(context);
      BOOST_CHECK_GE(boost::lexical_cast<double>(readLastData(context, "temp1").m_value), (double)temp1 - 1.0);
      BOOST_CHECK_LE(boost::lexical_cast<double>(readLastData(context, "temp1").m_value), (double)temp1 + 1.0);
      temp1 = boost::lexical_cast<double>(readLastData(context, "temp1").m_value);
      BOOST_CHECK_GE(boost::lexical_cast<double>(readLastData(context, "temp2").m_value), (double)temp2 - 1.0);
      BOOST_CHECK_LE(boost::lexical_cast<double>(readLastData(context, "temp2").m_value), (double)temp2 + 1.0);
      temp2 = boost::lexical_cast<double>(readLastData(context, "temp2").m_value);
      sensor.read();
   }
}

BOOST_AUTO_TEST_SUITE_END()
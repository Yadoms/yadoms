#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include <../../../../sources/shared/shared/plugin/yPluginApi/StandardCapacities.h>
#include <../../../../sources/shared/shared/plugin/yPluginApi/StandardUnits.h>
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
   virtual bool keywordExists(const std::string& device, const std::string& keyword) const { return false; }
   virtual bool keywordExists(const std::string& device, const yApi::historization::IHistorizable& keyword) const { return false; }
   virtual void declareKeyword(const std::string& device, const yApi::historization::IHistorizable& keyword, const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer)
   { Keyword kw = { device, keyword.getKeyword(), keyword.getCapacity(), details }; m_keywords[keyword.getKeyword()] = kw; }
   virtual void historizeData(const std::string& device, const yApi::historization::IHistorizable& keyword)
   { Data data = {device, keyword.getKeyword(), keyword.getCapacity(), keyword.formatValue() }; m_data.push_back(data); }
   virtual void historizeData(const std::string& device, std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > & dataVect) {  /* Not implemented */ }
   virtual const shared::plugin::information::IInformation& getInformation() const { return m_defaultInformation; }
   virtual shared::CDataContainer getConfiguration() const { return m_defaultConfiguration; }
   virtual void recordPluginEvent(PluginEventSeverity severity, const std::string & message) {}
   virtual const boost::filesystem::path getPluginPath() const { return boost::filesystem::initial_path<boost::filesystem::path>(); }


   //-----------------------------------------------------
   ///\brief Get a field value for a recipient ID
   ///\param    [in]    recipientId       The recipient ID containing the field
   ///\param    [in]    fieldName         The expected field ("phone", "email", etc...)
   ///\return                             The field value for specified recipient ID
   ///\throw shared::exception::CEmptyResult if recipient ID not found or field not found in recipient
   //-----------------------------------------------------
   virtual std::string getRecipientValue(int recipientId, const std::string& fieldName) const { return shared::CStringExtension::EmptyString; }
   virtual std::vector<int> findRecipientsFromField(const std::string& fieldName, const std::string& expectedFieldValue) const { return m_recipients; }
   virtual bool recipientFieldExists(const std::string& fieldName) const { return true; }
   virtual void createRecipientField(const std::string& fieldName, const std::string& checkRegex) {}

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
   std::vector<int> m_recipients;
};

void ckeckKeyword(boost::shared_ptr<CDefaultYPluginApiMock> context, const std::string& keyword, const std::string& device, const yApi::CStandardCapacity& capacity)
{
   std::map<std::string, CDefaultYPluginApiMock::Keyword>::const_iterator itKw = context->getKeywords().find(keyword);
   if (itKw == context->getKeywords().end())
      BOOST_ERROR(keyword + " keyword not found");

   CDefaultYPluginApiMock::Keyword kw = itKw->second;

   BOOST_CHECK_EQUAL(kw.m_device, device);
   BOOST_CHECK_EQUAL(kw.m_capacity, capacity.getName());
}

BOOST_AUTO_TEST_CASE(DeviceDeclaration)
{
   CFakeSensor sensor(sensorId);
   boost::shared_ptr<CDefaultYPluginApiMock> context(new CDefaultYPluginApiMock);

   sensor.declareDevice(context);

   // Check keywords declaration
   BOOST_CHECK_EQUAL(context->getKeywords().size(), (unsigned int)6);
   ckeckKeyword(context, "temp1", sensorId, yApi::CStandardCapacities::Temperature);
   ckeckKeyword(context, "temp2", sensorId, yApi::CStandardCapacities::Temperature);
   ckeckKeyword(context, "Battery", sensorId, yApi::CStandardCapacities::BatteryLevel);
   ckeckKeyword(context, "rssi", sensorId, yApi::CStandardCapacities::Rssi);
   ckeckKeyword(context, "dateTime", sensorId, yApi::CStandardCapacities::DateTime);
   ckeckKeyword(context, "current", sensorId, yApi::CStandardCapacities::Current);
}

const CDefaultYPluginApiMock::Data& readLastData(boost::shared_ptr<CDefaultYPluginApiMock> context, const std::string& keyword)
{
   static CDefaultYPluginApiMock::Data noData;
   if (context->getData().empty())
   {
      BOOST_FAIL("data not found");
      return noData;
   }

   for (std::vector<CDefaultYPluginApiMock::Data>::const_reverse_iterator itData = context->getData().rbegin() ; itData != context->getData().rend() ; ++itData)
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
   boost::shared_ptr<CDefaultYPluginApiMock> context(new CDefaultYPluginApiMock);

   sensor.historizeData(context);

   BOOST_CHECK_EQUAL(context->getData().size(), (unsigned int)6);
   BOOST_CHECK_EQUAL(readLastData(context, "temp1").m_device, sensorId);
   BOOST_CHECK_EQUAL(boost::lexical_cast<double>(readLastData(context, "temp1").m_value), 25.0);
   BOOST_CHECK_EQUAL(readLastData(context, "temp2").m_device, sensorId);
   BOOST_CHECK_EQUAL(boost::lexical_cast<double>(readLastData(context, "temp2").m_value), 10.0);
   BOOST_CHECK_EQUAL(readLastData(context, "Battery").m_device, sensorId);
   BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(context, "Battery").m_value), 100);
   BOOST_CHECK_EQUAL(readLastData(context, "rssi").m_device, sensorId);
   BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(context, "rssi").m_value), 50);
   BOOST_CHECK_EQUAL(readLastData(context, "current").m_device, sensorId);
   BOOST_CHECK_EQUAL(boost::lexical_cast<double>(readLastData(context, "current").m_value), 2.0);
   BOOST_CHECK_EQUAL(readLastData(context, "dateTime").m_device, sensorId);
   boost::posix_time::ptime readLastTime = boost::posix_time::time_from_string(readLastData(context, "dateTime").m_value);
   boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
   BOOST_CHECK_LE(readLastTime, now); // now-1s < dateTime <= now
   BOOST_CHECK_GT(readLastTime, now - boost::posix_time::seconds(1));

   sensor.read();
   sensor.historizeData(context);

   BOOST_CHECK_EQUAL(context->getData().size(), (unsigned int)12);
   BOOST_CHECK_EQUAL(readLastData(context, "temp1").m_device, sensorId);
   BOOST_CHECK_GE(boost::lexical_cast<double>(readLastData(context, "temp1").m_value), 24.0);
   BOOST_CHECK_LE(boost::lexical_cast<double>(readLastData(context, "temp1").m_value), 26.0);
   BOOST_CHECK_EQUAL(readLastData(context, "temp2").m_device, sensorId);
   BOOST_CHECK_GE(boost::lexical_cast<double>(readLastData(context, "temp2").m_value), 9.0);
   BOOST_CHECK_LE(boost::lexical_cast<double>(readLastData(context, "temp2").m_value), 11.0);
   BOOST_CHECK_EQUAL(readLastData(context, "Battery").m_device, sensorId);
   BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(context, "Battery").m_value), 99);
   BOOST_CHECK_EQUAL(readLastData(context, "rssi").m_device, sensorId);
   BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(context, "rssi").m_value), 50);
   BOOST_CHECK_EQUAL(readLastData(context, "current").m_device, sensorId);
   BOOST_CHECK_EQUAL(readLastData(context, "dateTime").m_device, sensorId);
   readLastTime = boost::posix_time::time_from_string(readLastData(context, "dateTime").m_value);
   now = boost::posix_time::microsec_clock::local_time();
   BOOST_CHECK_LE(readLastTime, now); // now-1s < dateTime <= now
   BOOST_CHECK_GT(readLastTime, now - boost::posix_time::seconds(1));
}

BOOST_AUTO_TEST_CASE(BatteryDecrease)
{
   CFakeSensor sensor(sensorId);
   boost::shared_ptr<CDefaultYPluginApiMock> context(new CDefaultYPluginApiMock);

   // Decrease from 100 at each read
   // Stop decrease at 20%
   for (int i = 100 ; i >= 0 ; --i)
   {
      sensor.historizeData(context);
      if (i >=20 )
         BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(context, "Battery").m_value), (int)i);
      else
         BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(context, "Battery").m_value), (int)20);
      sensor.read();
   }
}

BOOST_AUTO_TEST_CASE(TemperatureVariations)
{
   CFakeSensor sensor(sensorId);
   boost::shared_ptr<CDefaultYPluginApiMock> context(new CDefaultYPluginApiMock);

   // Loop of 100 tests
   // Temperature variation must be +/- 1° from previous value
   // First value is 25° for temp1, 10° for temp2
   double temp1 = 25.0, temp2 = 10.0;
   for (int i = 100 ; i >= 0 ; --i)
   {
      sensor.historizeData(context);
      BOOST_CHECK_GE(boost::lexical_cast<double>(readLastData(context, "temp1").m_value), (double)temp1 - 1.1); // Add some marging for float conversion
      BOOST_CHECK_LE(boost::lexical_cast<double>(readLastData(context, "temp1").m_value), (double)temp1 + 1.1);
      temp1 = boost::lexical_cast<double>(readLastData(context, "temp1").m_value);
      BOOST_CHECK_GE(boost::lexical_cast<double>(readLastData(context, "temp2").m_value), (double)temp2 - 1.1);
      BOOST_CHECK_LE(boost::lexical_cast<double>(readLastData(context, "temp2").m_value), (double)temp2 + 1.1);
      temp2 = boost::lexical_cast<double>(readLastData(context, "temp2").m_value);
      sensor.read();
   }
}

BOOST_AUTO_TEST_SUITE_END()
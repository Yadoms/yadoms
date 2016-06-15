#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include <../../../../sources/shared/shared/plugin/yPluginApi/StandardCapacities.h>
#include <../../../../sources/plugins/dev-FakePlugin/FakeSensor.h>
#include <../../../../sources/shared/shared/currentTime/Provider.h>

#include "../../mock/server/pluginSystem/DefaultYPluginApiMock.hpp"
#include "../../mock/shared/currentTime/DefaultCurrentTimeMock.hpp"

BOOST_AUTO_TEST_SUITE(TestFakeSensor)

static const std::string sensorId("MySensor");
static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

namespace yApi = shared::plugin::yPluginApi;

void ckeckKeyword(boost::shared_ptr<CDefaultYPluginApiMock> context, const std::string& keyword, const std::string& device, const yApi::CStandardCapacity& capacity)
{
   std::map<std::string, CDefaultYPluginApiMock::Keyword>::const_iterator itKw = context->getKeywords().find(keyword);
   if (itKw == context->getKeywords().end())
      BOOST_ERROR(keyword + " keyword not found");

   auto kw = itKw->second;

   BOOST_CHECK_EQUAL(kw.m_device, device);
   BOOST_CHECK_EQUAL(kw.m_capacity, capacity.getName());
}

BOOST_AUTO_TEST_CASE(DeviceDeclaration)
{
   CFakeSensor sensor(sensorId);
   auto context(boost::make_shared<CDefaultYPluginApiMock>());

   sensor.declareDevice(context);

   // Check keywords declaration
   BOOST_CHECK_EQUAL(context->getKeywords().size(), static_cast<unsigned int>(6));
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
   boost::shared_ptr<CDefaultYPluginApiMock> context(boost::make_shared<CDefaultYPluginApiMock>());

   sensor.historizeData(context);

   BOOST_CHECK_EQUAL(context->getData().size(), static_cast<unsigned int>(6));
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
   BOOST_CHECK_EQUAL(readLastTime, shared::currentTime::Provider::now());

   sensor.read();
   sensor.historizeData(context);

   BOOST_CHECK_EQUAL(context->getData().size(), static_cast<unsigned int>(12));
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
   BOOST_CHECK_EQUAL(readLastTime, shared::currentTime::Provider::now());
}

BOOST_AUTO_TEST_CASE(BatteryDecrease)
{
   CFakeSensor sensor(sensorId);
   boost::shared_ptr<CDefaultYPluginApiMock> context(boost::make_shared<CDefaultYPluginApiMock>());

   // Decrease from 100 at each read
   // Stop decrease at 20%
   for (int i = 100 ; i >= 0 ; --i)
   {
      sensor.historizeData(context);
      if (i >=20 )
         BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(context, "Battery").m_value), static_cast<int>(i));
      else
         BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(context, "Battery").m_value), static_cast<int>(20));
      sensor.read();
   }
}

BOOST_AUTO_TEST_CASE(TemperatureVariations)
{
   CFakeSensor sensor(sensorId);
   boost::shared_ptr<CDefaultYPluginApiMock> context(boost::make_shared<CDefaultYPluginApiMock>());

   // Loop of 100 tests
   // Temperature variation must be +/- 1° from previous value
   // First value is 25° for temp1, 10° for temp2
   double temp1 = 25.0, temp2 = 10.0;
   for (int i = 100 ; i >= 0 ; --i)
   {
      sensor.historizeData(context);
      BOOST_CHECK_GE(boost::lexical_cast<double>(readLastData(context, "temp1").m_value), static_cast<double>(temp1) - 1.1); // Add some marging for float conversion
      BOOST_CHECK_LE(boost::lexical_cast<double>(readLastData(context, "temp1").m_value), static_cast<double>(temp1) + 1.1);
      temp1 = boost::lexical_cast<double>(readLastData(context, "temp1").m_value);
      BOOST_CHECK_GE(boost::lexical_cast<double>(readLastData(context, "temp2").m_value), static_cast<double>(temp2) - 1.1);
      BOOST_CHECK_LE(boost::lexical_cast<double>(readLastData(context, "temp2").m_value), static_cast<double>(temp2) + 1.1);
      temp2 = boost::lexical_cast<double>(readLastData(context, "temp2").m_value);
      sensor.read();
   }
}

BOOST_AUTO_TEST_SUITE_END()
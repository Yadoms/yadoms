#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include <../../../../sources/shared/shared/plugin/yPluginApi/StandardCapacities.h>
#include <../../../../sources/plugins/dev-FakePlugin/FakeSensor.h>
#include <../../../../sources/shared/shared/currentTime/Provider.h>

#include "../../mock/server/pluginSystem/DefaultYPluginApiMock.hpp"
#include "../../mock/shared/currentTime/DefaultCurrentTimeMock.h"

BOOST_AUTO_TEST_SUITE(TestFakeSensor)

   static const std::string SensorId("MySensor");

   namespace yApi = shared::plugin::yPluginApi;

   void ckeckKeyword(boost::shared_ptr<CDefaultYPluginApiMock> api,
                     const std::string& keyword,
                     const std::string& device,
                     const yApi::CStandardCapacity& capacity)
   {
      const auto itKw = api->getKeywords().find(keyword);
      if (itKw == api->getKeywords().end())
         BOOST_ERROR(keyword + " keyword not found");

      auto kw = itKw->second;

      BOOST_CHECK_EQUAL(kw.m_device, device);
      BOOST_CHECK_EQUAL(kw.m_capacity, capacity.getName());
   }

   BOOST_AUTO_TEST_CASE(DeviceDeclaration)
   {
      CFakeSensor sensor(SensorId);
      auto api(boost::make_shared<CDefaultYPluginApiMock>());

      sensor.declareDevice(api);

      // Check keywords declaration
      BOOST_CHECK_EQUAL(api->getKeywords().size(), static_cast<unsigned int>(6));
      ckeckKeyword(api, "temp1", SensorId, yApi::CStandardCapacities::Temperature());
      ckeckKeyword(api, "temp2", SensorId, yApi::CStandardCapacities::Temperature());
      ckeckKeyword(api, "Battery", SensorId, yApi::CStandardCapacities::BatteryLevel());
      ckeckKeyword(api, "signalPower", SensorId, yApi::CStandardCapacities::SignalPower());
      ckeckKeyword(api, "dateTime", SensorId, yApi::CStandardCapacities::DateTime());
      ckeckKeyword(api, "current", SensorId, yApi::CStandardCapacities::Current());
   }

   const CDefaultYPluginApiMock::Data& readLastData(boost::shared_ptr<CDefaultYPluginApiMock> api,
                                                    const std::string& keyword)
   {
      static CDefaultYPluginApiMock::Data noData;
      if (api->getData().empty())
      {
         BOOST_FAIL("data not found");
         return noData;
      }

      for (auto itData = api->getData().rbegin(); itData != api->getData().rend(); ++itData)
      {
         if (itData->m_keyword == keyword)
            return *itData;
      }

      BOOST_FAIL("data not found");
      return noData; // Just to avoid compiler warning. No importance what is returned here, as test is already KO.
   }

   BOOST_AUTO_TEST_CASE(Historization)
   {
      auto api(boost::make_shared<CDefaultYPluginApiMock>());
      useTimeMock();

      CFakeSensor sensor(SensorId);
      sensor.historizeData(api);

      BOOST_CHECK_EQUAL(api->getData().size(), static_cast<unsigned int>(6));
      BOOST_CHECK_EQUAL(readLastData(api, "temp1").m_device, SensorId);
      BOOST_CHECK_EQUAL(boost::lexical_cast<double>(readLastData(api, "temp1").m_value), 25.0);
      BOOST_CHECK_EQUAL(readLastData(api, "temp2").m_device, SensorId);
      BOOST_CHECK_EQUAL(boost::lexical_cast<double>(readLastData(api, "temp2").m_value), 10.0);
      BOOST_CHECK_EQUAL(readLastData(api, "Battery").m_device, SensorId);
      BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(api, "Battery").m_value), 100);
      BOOST_CHECK_EQUAL(readLastData(api, "signalPower").m_device, SensorId);
      BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(api, "signalPower").m_value), 50);
      BOOST_CHECK_EQUAL(readLastData(api, "current").m_device, SensorId);
      BOOST_CHECK_EQUAL(boost::lexical_cast<double>(readLastData(api, "current").m_value), 2.0);
      BOOST_CHECK_EQUAL(readLastData(api, "dateTime").m_device, SensorId);
      auto readLastTime = boost::posix_time::time_from_string(readLastData(api, "dateTime").m_value);
      BOOST_CHECK_EQUAL(readLastTime, shared::currentTime::Provider().now());

      sensor.read();
      sensor.historizeData(api);

      BOOST_CHECK_EQUAL(api->getData().size(), static_cast<unsigned int>(12));
      BOOST_CHECK_EQUAL(readLastData(api, "temp1").m_device, SensorId);
      BOOST_CHECK_GE(boost::lexical_cast<double>(readLastData(api, "temp1").m_value), 24.0);
      BOOST_CHECK_LE(boost::lexical_cast<double>(readLastData(api, "temp1").m_value), 26.0);
      BOOST_CHECK_EQUAL(readLastData(api, "temp2").m_device, SensorId);
      BOOST_CHECK_GE(boost::lexical_cast<double>(readLastData(api, "temp2").m_value), 8.0);
      BOOST_CHECK_LE(boost::lexical_cast<double>(readLastData(api, "temp2").m_value), 12.0);
      BOOST_CHECK_EQUAL(readLastData(api, "Battery").m_device, SensorId);
      BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(api, "Battery").m_value), 99);
      BOOST_CHECK_EQUAL(readLastData(api, "signalPower").m_device, SensorId);
      BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(api, "signalPower").m_value), 50);
      BOOST_CHECK_EQUAL(readLastData(api, "current").m_device, SensorId);
      BOOST_CHECK_EQUAL(readLastData(api, "dateTime").m_device, SensorId);
      readLastTime = boost::posix_time::time_from_string(readLastData(api, "dateTime").m_value);
      BOOST_CHECK_EQUAL(readLastTime, shared::currentTime::Provider().now());
   }

   BOOST_AUTO_TEST_CASE(BatteryDecrease)
   {
      auto api(boost::make_shared<CDefaultYPluginApiMock>());

      CFakeSensor sensor(SensorId);

      // Decrease from 100 at each read
      // Stop decrease at 20%
      for (auto i = 100; i >= 0; --i)
      {
         sensor.historizeData(api);
         if (i >= 20)
            BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(api, "Battery").m_value), static_cast<int>(i));
         else
            BOOST_CHECK_EQUAL(boost::lexical_cast<int>(readLastData(api, "Battery").m_value), static_cast<int>(20));
         sensor.read();
      }
   }

   BOOST_AUTO_TEST_CASE(TemperatureVariations)
   {
      CFakeSensor sensor(SensorId);
      auto api(boost::make_shared<CDefaultYPluginApiMock>());

      // Loop of 100 tests
      // Temperature variation must be +/- 1° from previous value for temp1 and +/- 2° from previous value for temp12
      // First value is 25° for temp1, 10° for temp2
      auto temp1 = 25.0, temp2 = 10.0;
      for (auto i = 100; i >= 0; --i)
      {
         sensor.historizeData(api);
         BOOST_CHECK_GE(boost::lexical_cast<double>(readLastData(api, "temp1").m_value), static_cast<double>(temp1) - 1.1);
         // Add some marging for float conversion
         BOOST_CHECK_LE(boost::lexical_cast<double>(readLastData(api, "temp1").m_value), static_cast<double>(temp1) + 1.1);
         temp1 = boost::lexical_cast<double>(readLastData(api, "temp1").m_value);
         BOOST_CHECK_GE(boost::lexical_cast<double>(readLastData(api, "temp2").m_value), static_cast<double>(temp2) - 2.1);
         BOOST_CHECK_LE(boost::lexical_cast<double>(readLastData(api, "temp2").m_value), static_cast<double>(temp2) + 2.1);
         temp2 = boost::lexical_cast<double>(readLastData(api, "temp2").m_value);
         sensor.read();
      }
   }

BOOST_AUTO_TEST_SUITE_END()

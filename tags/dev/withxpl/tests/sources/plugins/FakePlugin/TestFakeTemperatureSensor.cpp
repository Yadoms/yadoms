#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/FakePlugin/FakeTemperatureSensor.h"

BOOST_AUTO_TEST_SUITE(TestFakeTemperatureSensor)

static const std::string sensorId("MyTemperatureSensor");

BOOST_AUTO_TEST_CASE(DefaultValues)
{
   CFakeTemperatureSensor sensor(sensorId);

   BOOST_CHECK_EQUAL(sensor.getDeviceId(), sensorId);
   BOOST_CHECK_EQUAL(sensor.getTemperature(), (double)25.0);
   BOOST_CHECK_EQUAL(sensor.getBatteryLevel(), (int)100);
   BOOST_CHECK_EQUAL(sensor.getRssi(), (int)50);
}

BOOST_AUTO_TEST_CASE(FirstRead)
{
   CFakeTemperatureSensor sensor(sensorId);

   BOOST_CHECK_EQUAL(sensor.getDeviceId(), sensorId);
   BOOST_CHECK_EQUAL(sensor.getTemperature(), (double)25.0);
   BOOST_CHECK_EQUAL(sensor.getBatteryLevel(), (int)100);
   BOOST_CHECK_EQUAL(sensor.getRssi(), (int)50);

   sensor.read();

   BOOST_CHECK_GE(sensor.getTemperature(), (double)24.0);
   BOOST_CHECK_LE(sensor.getTemperature(), (double)26.0);
   BOOST_CHECK_EQUAL(sensor.getBatteryLevel(), (int)99);
   BOOST_CHECK_EQUAL(sensor.getRssi(), (int)50);
}

BOOST_AUTO_TEST_CASE(BatteryDecrease)
{
   CFakeTemperatureSensor sensor(sensorId);

   // Decrease from 100 at each read
   // Stop decrease at 20%
   for (int i = 100 ; i >= 0 ; --i)
   {
      if (i >=20 )
         BOOST_CHECK_EQUAL(sensor.getBatteryLevel(), (int)i);
      else
         BOOST_CHECK_EQUAL(sensor.getBatteryLevel(), (int)20);
      sensor.read();
   }
}

BOOST_AUTO_TEST_CASE(TemperatureVariations)
{
   CFakeTemperatureSensor sensor(sensorId);

   // Loop of 100 tests
   // Temperature variation must be +/- 1° from previous value
   // First value is 25°
   double temperature = 25.0;
   for (int i = 100 ; i >= 0 ; --i)
   {
      sensor.read();
      BOOST_CHECK_GE(sensor.getTemperature(), (double)temperature - 1.0);
      BOOST_CHECK_LE(sensor.getTemperature(), (double)temperature + 1.0);
      temperature = sensor.getTemperature();
   }
}

BOOST_AUTO_TEST_SUITE_END()
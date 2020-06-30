#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/http/ssdp/DiscoveredDevice.h"
#include <shared/Log.h>

BOOST_AUTO_TEST_SUITE(TestDiscoveredDevice)

   BOOST_AUTO_TEST_CASE(InvalidContruction)
   {
      BOOST_CHECK_THROW(shared::http::ssdp::CDiscoveredDevice dd(""), std::runtime_error);
   }

   BOOST_AUTO_TEST_CASE(Lametric)
   {
      static const auto LametricSsdpAnswer = std::string(
         "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
         "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">"
         "<specVersion>"
         "<major>1</major>"
         "<minor>0</minor>"
         "</specVersion>"
         "<URLBase>https://192.168.1.10:443</URLBase>"
         "<device>"
         "<deviceType>urn:schemas-upnp-org:device:LaMetric:1</deviceType>"
         "<friendlyName>LaMetric Time (LM3989)</friendlyName>"
         "<manufacturer>LaMetric Inc.</manufacturer>"
         "<manufacturerURL>http://www.lametric.com</manufacturerURL>"
         "<modelDescription>LaMetric - internet connected clock and smart display</modelDescription>"
         "<modelName>LaMetric Time</modelName>"
         "<modelNumber>SA01</modelNumber>"
         "<modelURL>http://www.lametric.com</modelURL>"
         "<serialNumber>SA171202398900W00BS9</serialNumber>"
         "<serverId>27539</serverId>"
         "<deviceId>27539</deviceId>"
         "<UDN>uuid:ca964fe6-117e-4880-a303-3b0c13c125fc</UDN>"
         "</device>"
         "</root>");


      const shared::http::ssdp::CDiscoveredDevice dd(LametricSsdpAnswer);
      BOOST_CHECK_EQUAL(dd.ip(), "192.168.1.10");
      BOOST_CHECK_EQUAL(dd.deviceDescription()->get<std::string>("deviceType"),
                        "urn:schemas-upnp-org:device:LaMetric:1");
      BOOST_CHECK_EQUAL(dd.deviceDescription()->get<std::string>("friendlyName"), "LaMetric Time (LM3989)");
      BOOST_CHECK_EQUAL(dd.deviceDescription()->get<std::string>("manufacturer"), "LaMetric Inc.");
      BOOST_CHECK_EQUAL(dd.deviceDescription()->get<std::string>("manufacturerURL"), "http://www.lametric.com");
      BOOST_CHECK_EQUAL(dd.deviceDescription()->get<std::string>("modelDescription"),
                        "LaMetric - internet connected clock and smart display");
      BOOST_CHECK_EQUAL(dd.deviceDescription()->get<std::string>("modelName"), "LaMetric Time");
      BOOST_CHECK_EQUAL(dd.deviceDescription()->get<std::string>("modelNumber"), "SA01");
      BOOST_CHECK_EQUAL(dd.deviceDescription()->get<std::string>("modelURL"), "http://www.lametric.com");
      BOOST_CHECK_EQUAL(dd.deviceDescription()->get<std::string>("serialNumber"), "SA171202398900W00BS9");
      BOOST_CHECK_EQUAL(dd.deviceDescription()->get<int>("serverId"), 27539);
      BOOST_CHECK_EQUAL(dd.deviceDescription()->get<int>("deviceId"), 27539);
      BOOST_CHECK_EQUAL(dd.deviceDescription()->get<std::string>("UDN"), "uuid:ca964fe6-117e-4880-a303-3b0c13c125fc");
   }

BOOST_AUTO_TEST_SUITE_END()

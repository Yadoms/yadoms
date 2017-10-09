#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include "../../../../sources/plugins/OrangeBusiness/Decoder.h"
#include "../../mock/server/pluginSystem/DefaultYPluginApiMock.hpp"
#include <shared/exception/OutOfRange.hpp>


BOOST_AUTO_TEST_SUITE(TestOrangeBusiness)
  
namespace yApi = shared::plugin::yPluginApi;

   BOOST_AUTO_TEST_CASE(DecoderisFrameCompleteEmpty)
   {
      CDecoder decoder;
      shared::CDataContainer messageRecu;
      BOOST_REQUIRE_THROW(decoder.isFrameComplete(messageRecu), shared::exception::CInvalidParameter);
   }

   BOOST_AUTO_TEST_CASE(DecoderDeviceFrameEmpty)
   {
      CDecoder decoder;
      auto api(boost::make_shared<CDefaultYPluginApiMock>());

      shared::CDataContainer messageRecu;
      BOOST_REQUIRE_THROW(decoder.decodeDevicesMessage(api, messageRecu), shared::exception::CInvalidParameter);
   }

   BOOST_AUTO_TEST_CASE(DecoderDeviceFrameNominal)
   {
      auto api(boost::make_shared<CDefaultYPluginApiMock>());
      CDecoder decoder;

      shared::CDataContainer messageRecu;
      messageRecu.set("page", "0");
      messageRecu.set("size", "20");
      messageRecu.set("totalCount", "2");

      shared::CDataContainer device1, device2;

      // Device 1
      device1.set("devEUI", "0018B20000000272");
      device1.set("name", "DeviceTest2");
      device1.set("activationType", "OTAA"); 
      device1.set("profile", "SMTC/LoRaMoteClassA.2");
      device1.set("deviceStatus", "ACTIVATED");
      std::vector<std::string> tags;

      tags.push_back("Lyon");
      tags.push_back("Test");

      device1.set("tags", tags );
      device1.set("lastActivationTs", "2016-06-09T08:04:37.971Z");
      device1.set("lastCommunicationTs", "2016-06-03T15:55:36.944Z");
      device1.set("creationTs", "2016-06-03T15:20:53.803Z");
      device1.set("updateTs", "2016-06-09T08:04:37.971Z");

      // Device 2
      device2.set("devEUI", "0018B20000000274");
      device2.set("name", "DeviceTest1");
      device2.set("activationType", "OTAA");
      device2.set("profile", "LoRaWAN/DemonstratorClasseA");
      device2.set("deviceStatus", "ACTIVATED");
      tags.clear();

      tags.push_back("Lyon");
      tags.push_back("Test");

      device2.set("tags", tags);
      device2.set("lastActivationTs", "2016-06-09T08:04:37.971Z");
      device2.set("lastCommunicationTs", "2016-06-03T15:55:36.944Z");
      device2.set("creationTs", "2016-06-03T15:20:53.803Z");
      device2.set("updateTs", "2016-06-09T08:04:37.971Z");
      // add the devices
      std::vector<shared::CDataContainer> equipments;
      equipments.push_back(device1);
      equipments.push_back(device2);
      messageRecu.set("data", equipments);

	  std::map<std::string, boost::shared_ptr<equipments::IEquipment>> devicesRegistered = decoder.decodeDevicesMessage(api, messageRecu);
      BOOST_CHECK_EQUAL(decoder.isFrameComplete(messageRecu), true);

      BOOST_CHECK_EQUAL(devicesRegistered.size(), 2);
      BOOST_CHECK_NO_THROW(devicesRegistered.at("DeviceTest1")); // If present, no exception std::out_of_range
      BOOST_CHECK_NO_THROW(devicesRegistered.at("DeviceTest2"));
      BOOST_CHECK_EQUAL(devicesRegistered.at("DeviceTest2")->getEUI(), "0018B20000000272");
      BOOST_CHECK_EQUAL(devicesRegistered.at("DeviceTest1")->getEUI(), "0018B20000000274");
   }

   BOOST_AUTO_TEST_CASE(DecoderCommandListEmpty)
   {
      CDecoder decoder;
      shared::CDataContainer messageRecu;
      BOOST_REQUIRE_THROW(decoder.getLastData(messageRecu), shared::exception::CInvalidParameter);
   }

   BOOST_AUTO_TEST_CASE(DecoderCommandeListNominal)
   {
      shared::CDataContainer deviceInformation;
      CDecoder decoder;

      deviceInformation.set("page", 0);
      deviceInformation.set("size", 20);
      deviceInformation.set("totalCount", 2);

      std::vector<shared::CDataContainer> data;

      shared::CDataContainer command1, command2;

      command1.set("id", "5703cfa9e4b0b24cd6862865");
      command1.set("data", "01");
      command1.set("port", 1);
      command1.set("confirmed", true);
      command1.set("commandStatus", SENT);
      command1.set("creationTs", "2016-06-03T15:50:39.669Z");

      command2.set("id", "5703cfa9e4b0b24cd6862866");
      command2.set("data", "01");
      command2.set("port", 1);
      command2.set("confirmed", true);
      command2.set("commandStatus", SENT);
      command2.set("creationTs", "2016-06-03T15:50:39.669Z");

      data.push_back(command1);
      data.push_back(command2);
      deviceInformation.set("data", data);

      shared::CDataContainer response = decoder.getLastData(deviceInformation);

      BOOST_CHECK_EQUAL(boost::equal(response.get<std::string>("data"), "01"), true);
   }

   BOOST_AUTO_TEST_SUITE_END()
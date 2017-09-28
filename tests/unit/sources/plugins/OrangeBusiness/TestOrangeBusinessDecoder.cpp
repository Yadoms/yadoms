#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include "../../../../sources/plugins/OrangeBusiness/Decoder.h"
#include "../../mock/server/pluginSystem/DefaultYPluginApiMock.hpp"


BOOST_AUTO_TEST_SUITE(TestOrangeBusiness)
  
namespace yApi = shared::plugin::yPluginApi;

   BOOST_AUTO_TEST_CASE(DecoderDeviceFrame)
   {
      auto api(boost::make_shared<CDefaultYPluginApiMock>());
      CDecoder decoder(api);

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

      decoder.decodeDevicesMessage(api, messageRecu);
      BOOST_CHECK_EQUAL(decoder.isFrameComplete(messageRecu), true);

      std::vector<boost::shared_ptr<equipments::IEquipment>> devicesRegistered;
      devicesRegistered = decoder.getDevices();

      BOOST_CHECK_EQUAL(devicesRegistered.size(), 2);
   }

   BOOST_AUTO_TEST_SUITE_END()
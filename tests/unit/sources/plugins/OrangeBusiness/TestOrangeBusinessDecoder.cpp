#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include "../../../../sources/plugins/OrangeBusiness/Decoder.h"
#include "../../mock/server/pluginSystem/DefaultYPluginApiMock.hpp"


BOOST_AUTO_TEST_SUITE(TestOrangeBusiness)

   namespace yApi = shared::plugin::yPluginApi;

   BOOST_AUTO_TEST_CASE(DecoderisFrameCompleteEmpty)
   {
      CDecoder decoder;
      boost::shared_ptr<shared::CDataContainer> messageRecu = shared::CDataContainer::make();
      BOOST_REQUIRE_THROW(decoder.isFrameComplete(messageRecu), shared::exception::CInvalidParameter);
   }

   BOOST_AUTO_TEST_CASE(DecoderDeviceFrameEmpty)
   {
      CDecoder decoder;
      auto api(boost::make_shared<CDefaultYPluginApiMock>());

      boost::shared_ptr<shared::CDataContainer> messageRecu = shared::CDataContainer::make();
      BOOST_REQUIRE_THROW(decoder.decodeDevicesMessage(api, messageRecu), shared::exception::CInvalidParameter);
   }

   BOOST_AUTO_TEST_CASE(DecoderDeviceFrameNominal)
   {
      const auto api(boost::make_shared<CDefaultYPluginApiMock>());
      CDecoder decoder;

      boost::shared_ptr<shared::CDataContainer> messageRecu = shared::CDataContainer::make();
      messageRecu->set("page", 0);
      messageRecu->set("size", 20);
      messageRecu->set("totalCount", 2);

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

      device1.set("tags", tags);
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
      messageRecu->createArray("data");
      messageRecu->appendArray("data", device1);
      messageRecu->appendArray("data", device2);

      std::map<std::string, boost::shared_ptr<equipments::IEquipment>> devicesRegistered = decoder.decodeDevicesMessage(api, messageRecu);
      BOOST_CHECK_EQUAL(decoder.isFrameComplete(messageRecu), true);

      BOOST_CHECK_EQUAL(devicesRegistered.size(), static_cast<unsigned int>(2));
      BOOST_CHECK_NO_THROW(auto v1 = devicesRegistered.at("DeviceTest1")); // If present, no exception std::out_of_range
      BOOST_CHECK_NO_THROW(auto v2 = devicesRegistered.at("DeviceTest2"));
      BOOST_CHECK_EQUAL(devicesRegistered.at("DeviceTest2")->getEUI(), "0018B20000000272");
      BOOST_CHECK_EQUAL(devicesRegistered.at("DeviceTest1")->getEUI(), "0018B20000000274");
   }

   BOOST_AUTO_TEST_CASE(DecoderCommandeListNominal)
   {
      const std::string message = "[{\
         \"id\" : \"58e276370cf2cabaf8221840\",\
            \"streamId\" : \"urn:lora:70B3D532600013B5!uplink\",\
            \"timestamp\" : \"2017-04-03T16:20:07.513Z\",\
            \"model\" : \"lora_v0\",\
            \"value\" : {\
            \"port\" : 2,\
               \"fcnt\" : 1,\
               \"signalLevel\" : 5,\
               \"payload\" : \"03000000000004\"\
         },\
            \"tags\" : [],\
               \"metadata\" : {\
               \"source\" : \"urn:lora:70B3D532600013B5\",\
                  \"connector\" : \"lora\",\
                  \"network\" : {\
                  \"lora\" : {\
                     \"devEUI\" : \"70B3D532600013B5\",\
                        \"port\" : 2,\
                        \"fcnt\" : 1,\
                        \"rssi\" : -92.0,\
                        \"snr\" : 13.0,\
                        \"sf\" : 7,\
                        \"signalLevel\" : 5\
                  }\
               }\
            },\
               \"created\" : \"2017-04-03T16:20:07.793Z\"\
      }, {\
         \"id\" : \"58e272b20cf21b81b15cd1ea\",\
         \"streamId\" : \"urn:lora:70B3D532600013B5!uplink\",\
         \"timestamp\" : \"2017-04-03T16:05:05.741Z\",\
         \"model\" : \"lora_v0\",\
         \"value\" : {\
               \"port\" : 2,\
                  \"fcnt\" : 0,\
                  \"signalLevel\" : 5,\
                  \"payload\" : \"0400000000000000000004\"\
            },\
               \"tags\" : [],\
                  \"metadata\" : {\
                  \"source\" : \"urn:lora:70B3D532600013B5\",\
                     \"connector\" : \"lora\",\
                     \"network\" : {\
                     \"lora\" : {\
                        \"devEUI\" : \"70B3D532600013B5\",\
                           \"port\" : 2,\
                           \"fcnt\" : 0,\
                           \"rssi\" : -91.0,\
                           \"snr\" : 15.0,\
                           \"sf\" : 7,\
                           \"signalLevel\" : 5\
                     }\
                  }\
               },\
                  \"created\" : \"2017-04-03T16:05:06.026Z\"\
      } ]";

      boost::shared_ptr<shared::CDataContainer> messageRecu = shared::CDataContainer::make(message);

      CDecoder decoder;
      boost::shared_ptr<shared::CDataContainer> response = decoder.getLastData(messageRecu);

      BOOST_CHECK_EQUAL(response->get<std::string>("id") == "58e276370cf2cabaf8221840", true);
      BOOST_CHECK_EQUAL(response->get<std::string>("payload") == "03000000000004", true);
      BOOST_CHECK_EQUAL(response->get<std::string>("timestamp") == "2017-04-03T16:20:07.513Z", true);
      BOOST_CHECK_EQUAL(response->get<int>("signalLevel"), 5);
      BOOST_CHECK_EQUAL(response->get<double>("rssi"), -92);
      BOOST_CHECK_EQUAL(response->get<double>("snr"), 13);
   }

BOOST_AUTO_TEST_SUITE_END()

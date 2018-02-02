#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include "../../../../sources/plugins/OrangeBusiness/deviceManager.h"
#include "../../../../sources/plugins/OrangeBusiness/defaultEquipment.h"
#include "../../../../sources/plugins/OrangeBusiness/Decoder.h"
#include "../../mock/server/pluginSystem/DefaultYPluginApiMock.hpp"
#include <shared/exception/OutOfRange.hpp>

// Adding Mocks
#include "urlManagerMock.h"

// Addind tests Messages
#include "MessagesHelper.h"

BOOST_AUTO_TEST_SUITE(TestOrangeBusiness)
  
namespace yApi = shared::plugin::yPluginApi;

   BOOST_AUTO_TEST_CASE(EmptyDeviceManager)
   {
      std::map<std::string, boost::shared_ptr<equipments::IEquipment>> deviceList;
      CEquipmentManager deviceManager(deviceList);
      BOOST_CHECK_EQUAL(deviceManager.size(), 0);
   }

   BOOST_AUTO_TEST_CASE(DeviceManagerOneEquipment)
   {
      boost::shared_ptr<equipments::CDefaultEquipment> equipment = boost::make_shared<equipments::CDefaultEquipment>("Device1", "0018B20000000274");
      std::map<std::string, boost::shared_ptr<equipments::IEquipment>> deviceList = {
         { equipment->getName(), equipment }
      };

      CEquipmentManager deviceManager(deviceList);
      shared::CDataContainer messageRecu;
      BOOST_CHECK_EQUAL(deviceManager.size(), 1);
   }

   BOOST_AUTO_TEST_CASE(DecoderGetLastData)
   {
      auto m_urlManager = boost::make_shared<urlManagerMock>();
      auto api = boost::make_shared<CDefaultYPluginApiMock>();
      auto m_decoder = boost::make_shared<CDecoder>();

      std::string message = "[{\
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

      shared::CDataContainer messageRecu(message);
      m_urlManager->addRegisteredEquipmentsMessageReturned(MessageEquipmentInformation());
      m_urlManager->addMessagesForEquipment(messageRecu);

      boost::shared_ptr<equipments::CDefaultEquipment> equipment = boost::make_shared<equipments::CDefaultEquipment>("Device1", "0018B20000000274", api);
      std::map<std::string, boost::shared_ptr<equipments::IEquipment>> deviceList = {
         { equipment->getName(), equipment }
      };

      CEquipmentManager deviceManager(deviceList);
      BOOST_CHECK_EQUAL(deviceManager.size(), 1);

      deviceManager.refreshEquipment(api, equipment, m_urlManager, "ApiKey", m_decoder);
   }

   BOOST_AUTO_TEST_SUITE_END()
#include "stdafx.h"
#include "MessagesHelper.h"

shared::CDataContainerSharedPtr MessageEquipmentsList()
{
   shared::CDataContainerSharedPtr messageRecu = new_CDataContainerSharedPtr();
   messageRecu->set("page", "0");
   messageRecu->set("size", "20");
   messageRecu->set("totalCount", "2");

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

   return messageRecu;
}

shared::CDataContainerSharedPtr MessageEquipmentInformation()
{
   std::string message = " {\
      \"devEUI\" : \"70B3D532600013B5\",\
         \"name\" : \"Essai Emetteur JMB\",\
         \"activationType\" : \"OTAA\",\
         \"profile\" : \"Nemeus MK002-L-EU\",\
         \"deviceStatus\" : \"ACTIVATED\",\
         \"appEUI\" : \"70B3D53260000100\",\
         \"devAddr\" : \"1FC35BBC\",\
         \"tags\" : [],\
         \"lastActivationTs\" : \"2017-03-10T21:03:54.583Z\",\
         \"lastCommunicationTs\" : \"2017-04-03T16:20:07.513Z\",\
         \"lastSignalLevel\" : 5,\
         \"lastDlFcnt\" : 1,\
         \"creationTs\" : \"2017-03-10T21:03:54.583Z\",\
         \"updateTs\" : \"2017-03-10T21:03:54.583Z\"\
   }";

   return new_CDataContainerSharedPtrP(message);
}
#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include "../../../../sources/plugins/OrangeBusiness/deviceManager.h"
#include "../../../../sources/plugins/OrangeBusiness/defaultEquipment.h"
#include "../../mock/server/pluginSystem/DefaultYPluginApiMock.hpp"
#include <shared/exception/OutOfRange.hpp>


BOOST_AUTO_TEST_SUITE(TestOrangeBusiness)
  
namespace yApi = shared::plugin::yPluginApi;

   BOOST_AUTO_TEST_CASE(DecoderEmptyDeviceManager)
   {
      std::map<std::string, boost::shared_ptr<equipments::IEquipment>> deviceList;
      CEquipmentManager deviceManager(deviceList);
      BOOST_CHECK_EQUAL(deviceManager.size(), 0);
   }

   BOOST_AUTO_TEST_CASE(DecoderDeviceManagerOneEquipment)
   {
      equipments::CDefaultEquipment equipment("Device1", "0018B20000000274");
      std::map<std::string, boost::shared_ptr<equipments::IEquipment>> deviceList = { equipment.getName(), equipment };
      CEquipmentManager deviceManager(deviceList);
      shared::CDataContainer messageRecu;
      BOOST_CHECK_EQUAL(deviceManager.size(), 1);
   }

   BOOST_AUTO_TEST_SUITE_END()
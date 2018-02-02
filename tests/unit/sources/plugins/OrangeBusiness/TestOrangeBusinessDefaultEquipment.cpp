#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include "../../../../sources/plugins/OrangeBusiness/DefaultEquipment.h"

BOOST_AUTO_TEST_SUITE(TestOrangeBusiness)
  
namespace yApi = shared::plugin::yPluginApi;

BOOST_AUTO_TEST_CASE(DefaultEquipementConstructor)
{
   equipments::CDefaultEquipment equipment("Device1", "0018B20000000274");
   std::cout << equipment.getName();
   BOOST_CHECK_EQUAL(equipment.getName() == "Device1", true);
   BOOST_CHECK_EQUAL(equipment.getEUI() == "0018B20000000274", true);
}

BOOST_AUTO_TEST_SUITE_END()
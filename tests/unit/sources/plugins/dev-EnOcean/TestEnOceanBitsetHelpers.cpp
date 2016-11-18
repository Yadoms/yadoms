#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/dev-EnOcean/profiles/bitsetHelpers.hpp"

// Includes needed to compile the test
#include "../../testCommon/fileSystem.h"



BOOST_AUTO_TEST_SUITE(TestEnOceanBitsetHelpers)

BOOST_AUTO_TEST_CASE(NominalConversion)
{
   const std::vector<unsigned char> byteArray = { 0x01, 0x02, 0x55, 0xAA, 0xFF };
   const boost::dynamic_bitset<> expectedBitset =
   {
      false, false, false, false, false, false, false, true, // 0x01
      false, false, false, false, false, false, true, false, // 0x02
      false, true, false, true, false, true, false, true, // 0x55
      true, false, true, false, true, false, true, false, // 0xAA
      true, true, true, true, true, true, true, true, // 0xFF
   };

   

   auto bitset = bitset_from_bytes(byteArray);
   for (const auto bit: bitset)
}

BOOST_AUTO_TEST_SUITE_END()
#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/dev-EnOcean/profiles/bitsetHelpers.hpp"

// Includes needed to compile the test
#include "../../testCommon/fileSystem.h"


boost::dynamic_bitset<> initBitSet(const bool bitArray[],
                                   const size_t bitArraySize)
{
   boost::dynamic_bitset<> bitset(bitArraySize);
   for (size_t bitIndex = 0; bitIndex < bitArraySize; ++bitIndex)
      bitset[bitIndex] = bitArray[bitIndex];
   return bitset;
}

#define _initBitSet(bitArray) initBitSet(bitArray,sizeof(bitArray)/sizeof(bitArray[0]))


BOOST_AUTO_TEST_SUITE(TestEnOceanBitsetHelpers)

   BOOST_AUTO_TEST_CASE(NominalConversion)
   {
      const std::vector<unsigned char> byteArray = {0x01, 0x02, 0x55, 0xAA, 0xFF};
      const bool bitArray[]
      {
         false, false, false, false, false, false, false, true, // 0x01
         false, false, false, false, false, false, true, false, // 0x02
         false, true, false, true, false, true, false, true, // 0x55
         true, false, true, false, true, false, true, false, // 0xAA
         true, true, true, true, true, true, true, true, // 0xFF
      };
      const auto expectedBitset(_initBitSet(bitArray));

      // From bytes to bitset
      auto bitset = bitset_from_bytes(byteArray);
      BOOST_CHECK_EQUAL(bitset, expectedBitset);

      // From bitset to bytes
      auto newByteArray = bitset_to_bytes(bitset);
      BOOST_CHECK_EQUAL_COLLECTIONS(newByteArray.begin(), newByteArray.end(),
                                    byteArray.begin(), byteArray.end());
   }

BOOST_AUTO_TEST_SUITE_END()

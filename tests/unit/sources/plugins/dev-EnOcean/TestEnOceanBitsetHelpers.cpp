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

BOOST_AUTO_TEST_CASE(Extract)
{
   boost::dynamic_bitset<> bitset(10);

   BOOST_CHECK_EQUAL(bitset_extract(bitset, 0, 10), static_cast<unsigned int>(0));
   BOOST_CHECK_EQUAL(bitset_extract(bitset, 5, 1), static_cast<unsigned int>(0));

   bitset[3] = bitset[5] = true;

   BOOST_CHECK_EQUAL(bitset_extract(bitset, 2, 1), static_cast<unsigned int>(0));
   BOOST_CHECK_EQUAL(bitset_extract(bitset, 3, 1), static_cast<unsigned int>(1));
   BOOST_CHECK_EQUAL(bitset_extract(bitset, 4, 1), static_cast<unsigned int>(0));
   BOOST_CHECK_EQUAL(bitset_extract(bitset, 5, 1), static_cast<unsigned int>(1));
   BOOST_CHECK_EQUAL(bitset_extract(bitset, 6, 1), static_cast<unsigned int>(0));

   BOOST_CHECK_EQUAL(bitset_extract(bitset, 0, 10), static_cast<unsigned int>(80));

   BOOST_CHECK_EQUAL(bitset_extract(bitset, 2, 2), static_cast<unsigned int>(1));
   BOOST_CHECK_EQUAL(bitset_extract(bitset, 3, 2), static_cast<unsigned int>(2));
}

BOOST_AUTO_TEST_CASE(Insert)
{
   boost::dynamic_bitset<> bitset(10);

   bitset_insert(bitset, 0, 10, 0x2AA);
   BOOST_CHECK_EQUAL(bitset[0], true);
   BOOST_CHECK_EQUAL(bitset[1], false);
   BOOST_CHECK_EQUAL(bitset[2], true);
   BOOST_CHECK_EQUAL(bitset[3], false);
   BOOST_CHECK_EQUAL(bitset[4], true);
   BOOST_CHECK_EQUAL(bitset[5], false);
   BOOST_CHECK_EQUAL(bitset[6], true);
   BOOST_CHECK_EQUAL(bitset[7], false);
   BOOST_CHECK_EQUAL(bitset[8], true);
   BOOST_CHECK_EQUAL(bitset[9], false);

   bitset.reset();
   bitset_insert(bitset, 3, 1, 1);
   BOOST_CHECK_EQUAL(bitset[0], false);
   BOOST_CHECK_EQUAL(bitset[1], false);
   BOOST_CHECK_EQUAL(bitset[2], false);
   BOOST_CHECK_EQUAL(bitset[3], true);
   BOOST_CHECK_EQUAL(bitset[4], false);
   BOOST_CHECK_EQUAL(bitset[5], false);
   BOOST_CHECK_EQUAL(bitset[6], false);
   BOOST_CHECK_EQUAL(bitset[7], false);
   BOOST_CHECK_EQUAL(bitset[8], false);
   BOOST_CHECK_EQUAL(bitset[9], false);
   bitset_insert(bitset, 3, false);
   BOOST_CHECK_EQUAL(bitset[0], false);
   BOOST_CHECK_EQUAL(bitset[1], false);
   BOOST_CHECK_EQUAL(bitset[2], false);
   BOOST_CHECK_EQUAL(bitset[3], false);
   BOOST_CHECK_EQUAL(bitset[4], false);
   BOOST_CHECK_EQUAL(bitset[5], false);
   BOOST_CHECK_EQUAL(bitset[6], false);
   BOOST_CHECK_EQUAL(bitset[7], false);
   BOOST_CHECK_EQUAL(bitset[8], false);
   BOOST_CHECK_EQUAL(bitset[9], false);

   bitset.reset();
   bitset_insert(bitset, 5, 3, 5);
   BOOST_CHECK_EQUAL(bitset[0], false);
   BOOST_CHECK_EQUAL(bitset[1], false);
   BOOST_CHECK_EQUAL(bitset[2], false);
   BOOST_CHECK_EQUAL(bitset[3], false);
   BOOST_CHECK_EQUAL(bitset[4], false);
   BOOST_CHECK_EQUAL(bitset[5], true);
   BOOST_CHECK_EQUAL(bitset[6], false);
   BOOST_CHECK_EQUAL(bitset[7], true);
   BOOST_CHECK_EQUAL(bitset[8], false);
   BOOST_CHECK_EQUAL(bitset[9], false);
   bitset_insert(bitset, 4, true);
   BOOST_CHECK_EQUAL(bitset[0], false);
   BOOST_CHECK_EQUAL(bitset[1], false);
   BOOST_CHECK_EQUAL(bitset[2], false);
   BOOST_CHECK_EQUAL(bitset[3], false);
   BOOST_CHECK_EQUAL(bitset[4], true);
   BOOST_CHECK_EQUAL(bitset[5], true);
   BOOST_CHECK_EQUAL(bitset[6], false);
   BOOST_CHECK_EQUAL(bitset[7], true);
   BOOST_CHECK_EQUAL(bitset[8], false);
   BOOST_CHECK_EQUAL(bitset[9], false);
   bitset_insert(bitset, 5, 4, 14);
   BOOST_CHECK_EQUAL(bitset[0], false);
   BOOST_CHECK_EQUAL(bitset[1], false);
   BOOST_CHECK_EQUAL(bitset[2], false);
   BOOST_CHECK_EQUAL(bitset[3], false);
   BOOST_CHECK_EQUAL(bitset[4], true);
   BOOST_CHECK_EQUAL(bitset[5], true);
   BOOST_CHECK_EQUAL(bitset[6], true);
   BOOST_CHECK_EQUAL(bitset[7], true);
   BOOST_CHECK_EQUAL(bitset[8], false);
   BOOST_CHECK_EQUAL(bitset[9], false);
   bitset_insert(bitset, 6, 2, 2);
   BOOST_CHECK_EQUAL(bitset[0], false);
   BOOST_CHECK_EQUAL(bitset[1], false);
   BOOST_CHECK_EQUAL(bitset[2], false);
   BOOST_CHECK_EQUAL(bitset[3], false);
   BOOST_CHECK_EQUAL(bitset[4], true);
   BOOST_CHECK_EQUAL(bitset[5], true);
   BOOST_CHECK_EQUAL(bitset[6], true);
   BOOST_CHECK_EQUAL(bitset[7], false);
   BOOST_CHECK_EQUAL(bitset[8], false);
   BOOST_CHECK_EQUAL(bitset[9], false);
}

BOOST_AUTO_TEST_CASE(InsertThenExtract)
{
   boost::dynamic_bitset<> bitset(10);

   bitset_insert(bitset, 2, 4, 5);

   BOOST_CHECK_EQUAL(bitset_extract(bitset, 0, 2), static_cast<unsigned int>(0));
   BOOST_CHECK_EQUAL(bitset_extract(bitset, 2, 4), static_cast<unsigned int>(5));
   BOOST_CHECK_EQUAL(bitset_extract(bitset, 6, 4), static_cast<unsigned int>(0));
}

BOOST_AUTO_TEST_SUITE_END()

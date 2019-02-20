#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/EnOcean/profiles/bitsetHelpers.hpp"


boost::dynamic_bitset<> initBitSet(const bool bitArray[],
                                   const size_t bitArraySize)
{
   boost::dynamic_bitset<> bitSet(bitArraySize);
   for (size_t bitIndex = 0; bitIndex < bitArraySize; ++bitIndex)
      bitSet[bitIndex] = bitArray[bitIndex];
   return bitSet;
}

#define INIT_BIT_SET(bitArray) initBitSet(bitArray,sizeof(bitArray)/sizeof(bitArray[0]))


BOOST_AUTO_TEST_SUITE(TestBitsetHelpers)

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
   const auto expectedBitSet(INIT_BIT_SET(bitArray));

   // From bytes to bits set
   const auto bitSet = bitset_from_bytes(byteArray);
   BOOST_CHECK_EQUAL(bitSet, expectedBitSet);

   // From bits set to bytes
   auto newByteArray = bitset_to_bytes(bitSet);
   BOOST_CHECK_EQUAL_COLLECTIONS(newByteArray.begin(), newByteArray.end(),
                                 byteArray.begin(), byteArray.end());
}

BOOST_AUTO_TEST_CASE(Extract)
{
   boost::dynamic_bitset<> bitSet(10);

   BOOST_CHECK_EQUAL(bitset_extract(bitSet, 0, 10), static_cast<unsigned int>(0));
   BOOST_CHECK_EQUAL(bitset_extract(bitSet, 5, 1), static_cast<unsigned int>(0));

   bitSet[3] = bitSet[5] = true;

   BOOST_CHECK_EQUAL(bitset_extract(bitSet, 2, 1), static_cast<unsigned int>(0));
   BOOST_CHECK_EQUAL(bitset_extract(bitSet, 3, 1), static_cast<unsigned int>(1));
   BOOST_CHECK_EQUAL(bitset_extract(bitSet, 4, 1), static_cast<unsigned int>(0));
   BOOST_CHECK_EQUAL(bitset_extract(bitSet, 5, 1), static_cast<unsigned int>(1));
   BOOST_CHECK_EQUAL(bitset_extract(bitSet, 6, 1), static_cast<unsigned int>(0));

   BOOST_CHECK_EQUAL(bitset_extract(bitSet, 0, 10), static_cast<unsigned int>(80));

   BOOST_CHECK_EQUAL(bitset_extract(bitSet, 2, 2), static_cast<unsigned int>(1));
   BOOST_CHECK_EQUAL(bitset_extract(bitSet, 3, 2), static_cast<unsigned int>(2));
}

BOOST_AUTO_TEST_CASE(Insert)
{
   boost::dynamic_bitset<> bitSet(10);

   bitset_insert(bitSet, 0, 10, 0x2AA);
   BOOST_CHECK_EQUAL(bitSet[0], true);
   BOOST_CHECK_EQUAL(bitSet[1], false);
   BOOST_CHECK_EQUAL(bitSet[2], true);
   BOOST_CHECK_EQUAL(bitSet[3], false);
   BOOST_CHECK_EQUAL(bitSet[4], true);
   BOOST_CHECK_EQUAL(bitSet[5], false);
   BOOST_CHECK_EQUAL(bitSet[6], true);
   BOOST_CHECK_EQUAL(bitSet[7], false);
   BOOST_CHECK_EQUAL(bitSet[8], true);
   BOOST_CHECK_EQUAL(bitSet[9], false);

   bitSet.reset();
   bitset_insert(bitSet, 3, 1, 1);
   BOOST_CHECK_EQUAL(bitSet[0], false);
   BOOST_CHECK_EQUAL(bitSet[1], false);
   BOOST_CHECK_EQUAL(bitSet[2], false);
   BOOST_CHECK_EQUAL(bitSet[3], true);
   BOOST_CHECK_EQUAL(bitSet[4], false);
   BOOST_CHECK_EQUAL(bitSet[5], false);
   BOOST_CHECK_EQUAL(bitSet[6], false);
   BOOST_CHECK_EQUAL(bitSet[7], false);
   BOOST_CHECK_EQUAL(bitSet[8], false);
   BOOST_CHECK_EQUAL(bitSet[9], false);
   bitset_insert(bitSet, 3, false);
   BOOST_CHECK_EQUAL(bitSet[0], false);
   BOOST_CHECK_EQUAL(bitSet[1], false);
   BOOST_CHECK_EQUAL(bitSet[2], false);
   BOOST_CHECK_EQUAL(bitSet[3], false);
   BOOST_CHECK_EQUAL(bitSet[4], false);
   BOOST_CHECK_EQUAL(bitSet[5], false);
   BOOST_CHECK_EQUAL(bitSet[6], false);
   BOOST_CHECK_EQUAL(bitSet[7], false);
   BOOST_CHECK_EQUAL(bitSet[8], false);
   BOOST_CHECK_EQUAL(bitSet[9], false);

   bitSet.reset();
   bitset_insert(bitSet, 5, 3, 5);
   BOOST_CHECK_EQUAL(bitSet[0], false);
   BOOST_CHECK_EQUAL(bitSet[1], false);
   BOOST_CHECK_EQUAL(bitSet[2], false);
   BOOST_CHECK_EQUAL(bitSet[3], false);
   BOOST_CHECK_EQUAL(bitSet[4], false);
   BOOST_CHECK_EQUAL(bitSet[5], true);
   BOOST_CHECK_EQUAL(bitSet[6], false);
   BOOST_CHECK_EQUAL(bitSet[7], true);
   BOOST_CHECK_EQUAL(bitSet[8], false);
   BOOST_CHECK_EQUAL(bitSet[9], false);
   bitset_insert(bitSet, 4, true);
   BOOST_CHECK_EQUAL(bitSet[0], false);
   BOOST_CHECK_EQUAL(bitSet[1], false);
   BOOST_CHECK_EQUAL(bitSet[2], false);
   BOOST_CHECK_EQUAL(bitSet[3], false);
   BOOST_CHECK_EQUAL(bitSet[4], true);
   BOOST_CHECK_EQUAL(bitSet[5], true);
   BOOST_CHECK_EQUAL(bitSet[6], false);
   BOOST_CHECK_EQUAL(bitSet[7], true);
   BOOST_CHECK_EQUAL(bitSet[8], false);
   BOOST_CHECK_EQUAL(bitSet[9], false);
   bitset_insert(bitSet, 5, 4, 14);
   BOOST_CHECK_EQUAL(bitSet[0], false);
   BOOST_CHECK_EQUAL(bitSet[1], false);
   BOOST_CHECK_EQUAL(bitSet[2], false);
   BOOST_CHECK_EQUAL(bitSet[3], false);
   BOOST_CHECK_EQUAL(bitSet[4], true);
   BOOST_CHECK_EQUAL(bitSet[5], true);
   BOOST_CHECK_EQUAL(bitSet[6], true);
   BOOST_CHECK_EQUAL(bitSet[7], true);
   BOOST_CHECK_EQUAL(bitSet[8], false);
   BOOST_CHECK_EQUAL(bitSet[9], false);
   bitset_insert(bitSet, 6, 2, 2);
   BOOST_CHECK_EQUAL(bitSet[0], false);
   BOOST_CHECK_EQUAL(bitSet[1], false);
   BOOST_CHECK_EQUAL(bitSet[2], false);
   BOOST_CHECK_EQUAL(bitSet[3], false);
   BOOST_CHECK_EQUAL(bitSet[4], true);
   BOOST_CHECK_EQUAL(bitSet[5], true);
   BOOST_CHECK_EQUAL(bitSet[6], true);
   BOOST_CHECK_EQUAL(bitSet[7], false);
   BOOST_CHECK_EQUAL(bitSet[8], false);
   BOOST_CHECK_EQUAL(bitSet[9], false);
}

BOOST_AUTO_TEST_CASE(InsertThenExtract)
{
   boost::dynamic_bitset<> bitSet(10);

   bitset_insert(bitSet, 2, 4, 5);

   BOOST_CHECK_EQUAL(bitset_extract(bitSet, 0, 2), static_cast<unsigned int>(0));
   BOOST_CHECK_EQUAL(bitset_extract(bitSet, 2, 4), static_cast<unsigned int>(5));
   BOOST_CHECK_EQUAL(bitset_extract(bitSet, 6, 4), static_cast<unsigned int>(0));
}

BOOST_AUTO_TEST_SUITE_END()

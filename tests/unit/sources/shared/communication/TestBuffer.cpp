#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/communication/Buffer.hpp"

BOOST_AUTO_TEST_SUITE(TestBuffer)

   BOOST_AUTO_TEST_CASE(ByteBufferConstructionFromArray)
   {
      unsigned char input[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};

      shared::communication::CByteBuffer buffer(input, sizeof(input));
      BOOST_CHECK_EQUAL(buffer.size(), sizeof(input)) ; // Check size
      BOOST_CHECK_NE(buffer.begin(), &input[0]) ; // Check that a copy was done
      for (auto index = 0; index < buffer.size(); ++index) // Check content
      BOOST_CHECK_EQUAL(buffer[index], input[index]) ;
   }

   BOOST_AUTO_TEST_CASE(ByteBufferCopyConstruction)
   {
      unsigned char tab[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
      shared::communication::CByteBuffer buffer(tab, sizeof(tab));

      auto buffer2(buffer);

      BOOST_CHECK_EQUAL(buffer2.size(), buffer.size()); // Check size
      BOOST_CHECK_NE(&buffer[0], &buffer2[0]); // Check that a copy was done
      BOOST_CHECK_EQUAL_COLLECTIONS(buffer.begin(), buffer.end(),
         buffer2.begin(), buffer2.end());
   }

   BOOST_AUTO_TEST_CASE(ByteBufferCopy)
   {
      unsigned char tab[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
      shared::communication::CByteBuffer buffer(tab, sizeof(tab));

      shared::communication::CByteBuffer buffer2(128);
      BOOST_CHECK_EQUAL(buffer2.size(), static_cast<size_t>(128)); // Check size

      buffer2 = buffer;

      BOOST_CHECK_EQUAL(buffer2.size(), buffer.size()) ; // Check size
      BOOST_CHECK_NE(&buffer[0], &buffer2[0]) ; // Check that a copy was done
      BOOST_CHECK_EQUAL_COLLECTIONS(buffer.begin(), buffer.end(),
         buffer2.begin(), buffer2.end());
   }

   BOOST_AUTO_TEST_CASE(ByteBufferConstructionFromVector)
   {
      std::vector<unsigned char> input{0x01, 0x02,0x03,0x04,0x05,0x06};

      shared::communication::CByteBuffer buffer(input);

      // Check buffer content
      BOOST_CHECK_EQUAL(buffer.size(), input.size()) ; // Check size
      BOOST_CHECK_NE(buffer.begin(), &input[0]) ; // Check that a copy was done
      for (auto index = 0; index < buffer.size(); ++index) // Check content
      BOOST_CHECK_EQUAL(buffer[index], input[index]) ;
   }

   BOOST_AUTO_TEST_CASE(ByteBufferCopyFromVector)
   {
      std::vector<unsigned char> input{0x01, 0x02, 0x03, 0x04, 0x05, 0x06};

      shared::communication::CByteBuffer buffer(128);
      BOOST_CHECK_EQUAL(buffer.size(), static_cast<size_t>(128)) ; // Check size

      buffer = input;

      // Check buffer content
      BOOST_CHECK_EQUAL(buffer.size(), input.size()) ; // Check size
      BOOST_CHECK_NE(buffer.begin(), &input[0]) ; // Check that a copy was done
      BOOST_CHECK_EQUAL_COLLECTIONS(buffer.begin(), buffer.end(),
         input.begin(), input.end());
   }

   BOOST_AUTO_TEST_SUITE_END()


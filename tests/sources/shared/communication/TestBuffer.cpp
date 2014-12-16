#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/communication/Buffer.hpp"

BOOST_AUTO_TEST_SUITE(TestBuffer)

BOOST_AUTO_TEST_CASE(ByteBufferNominal)
{
   unsigned char tab[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
   size_t tabSize = sizeof(tab);
   shared::communication::CByteBuffer buffer(tabSize);
   memcpy(buffer.begin(), tab, tabSize);

   // Check buffer content
   BOOST_CHECK_NE(buffer.begin(), tab);                         // Ensure that data were copied
   BOOST_CHECK_EQUAL(buffer.size(), tabSize);                     // Check size
   for (size_t i = 0 ; i < tabSize ; ++i)
   {
      BOOST_CHECK_EQUAL(buffer.begin()[i], tab[i]);             // Check content
   }
   BOOST_CHECK_EQUAL(memcmp(buffer.begin(), tab, tabSize), 0);  // Other check content method
}

shared::communication::CByteBuffer getBuffer()
{
   unsigned char tab[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
   size_t tabSize = sizeof(tab);
   shared::communication::CByteBuffer buffer(tabSize);
   memcpy(buffer.begin(), tab, tabSize);
   return buffer;
}

void passBufferByRef(shared::communication::CByteBuffer& buffer)
{
   // Check buffer content
   unsigned char sameTab[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
   BOOST_CHECK_EQUAL(buffer.size(), sizeof(sameTab));                         // Check size
   BOOST_CHECK_EQUAL(memcmp(buffer.begin(), sameTab, sizeof(sameTab)), 0);  // Check content
}

BOOST_AUTO_TEST_CASE(ByteBufferCopy)
{
   shared::communication::CByteBuffer buffer = getBuffer();

   // Check buffer content
   unsigned char sameTab[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
   BOOST_CHECK_EQUAL(buffer.size(), sizeof(sameTab));                         // Check size
   BOOST_CHECK_EQUAL(memcmp(buffer.begin(), sameTab, sizeof(sameTab)), 0);    // Check content

   // Pass buffer by reference
   passBufferByRef(buffer);

   {
      shared::communication::CByteBuffer buffer2 = buffer;
      BOOST_CHECK(buffer2.begin() != buffer.begin());                         // Ensure that data were copied
      BOOST_CHECK_EQUAL(buffer2.size(), sizeof(sameTab));                     // Check size
      BOOST_CHECK(!memcmp(buffer2.begin(), sameTab, sizeof(sameTab)));        // Check content
   }

   // buffer2 is now deleted, ensure that data were not lost
   BOOST_CHECK_EQUAL(buffer.size(), sizeof(sameTab));                         // Check size
   BOOST_CHECK_EQUAL(memcmp(buffer.begin(), sameTab, sizeof(sameTab)), 0);    // Check content
}

BOOST_AUTO_TEST_CASE(ByteBufferCopy2)
{
   shared::communication::CByteBuffer* buffer = new shared::communication::CByteBuffer(getBuffer());

   // Check buffer content
   unsigned char sameTab[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
   BOOST_CHECK_EQUAL(buffer->size(), sizeof(sameTab));                         // Check size
   BOOST_CHECK_EQUAL(memcmp(buffer->begin(), sameTab, sizeof(sameTab)), 0);  // Check content

   shared::communication::CByteBuffer buffer2 = *buffer;
   delete buffer;
   BOOST_CHECK_EQUAL(buffer2.size(), sizeof(sameTab));                         // Check size
   BOOST_CHECK_EQUAL(memcmp(buffer2.begin(), sameTab, sizeof(sameTab)), 0);  // Check content
}

BOOST_AUTO_TEST_SUITE_END()
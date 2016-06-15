#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/Rfxcom/Buffer.hpp"

BOOST_AUTO_TEST_SUITE(TestBuffer)

BOOST_AUTO_TEST_CASE(ByteBufferNominal)
{
   unsigned char tab[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
   size_t tabSize = sizeof(tab);
   CByteBuffer buffer(tab, tabSize);

   // Check buffer content
   BOOST_CHECK_NE(buffer.content(), tab);                         // Ensure that data were copied
   BOOST_CHECK_EQUAL(buffer.size(), tabSize);                     // Check size
   for (size_t i = 0 ; i < tabSize ; ++i)
   {
      BOOST_CHECK_EQUAL(buffer.content()[i], tab[i]);             // Check content
   }
   BOOST_CHECK_EQUAL(memcmp(buffer.content(), tab, tabSize), 0);  // Other check content method
}

CByteBuffer getBuffer()
{
   unsigned char tab[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
   size_t tabSize = sizeof(tab);
   return CByteBuffer (tab, tabSize);
}

void passBufferByRef(CByteBuffer& buffer)
{
   // Check buffer content
   unsigned char sameTab[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
   BOOST_CHECK_EQUAL(buffer.size(), sizeof(sameTab));                         // Check size
   BOOST_CHECK_EQUAL(memcmp(buffer.content(), sameTab, sizeof(sameTab)), 0);  // Check content
}

BOOST_AUTO_TEST_CASE(ByteBufferCopy)
{
   CByteBuffer buffer = getBuffer();

   // Check buffer content
   unsigned char sameTab[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
   BOOST_CHECK_EQUAL(buffer.size(), sizeof(sameTab));                         // Check size
   BOOST_CHECK_EQUAL(memcmp(buffer.content(), sameTab, sizeof(sameTab)), 0);  // Check content

   // Pass buffer by reference
   passBufferByRef(buffer);

   {
      CByteBuffer buffer2 = buffer;
      BOOST_CHECK_EQUAL(buffer2.content(), buffer.content());                     // Ensure that data were not-copied (just ref-count inceremented)
      BOOST_CHECK_EQUAL(buffer2.size(), sizeof(sameTab));                         // Check size
      BOOST_CHECK_EQUAL(memcmp(buffer2.content(), sameTab, sizeof(sameTab)), 0);  // Check content
   }

   // buffer2 is now deleted, ensure that data were not lost
   BOOST_CHECK_EQUAL(buffer.size(), sizeof(sameTab));                         // Check size
   BOOST_CHECK_EQUAL(memcmp(buffer.content(), sameTab, sizeof(sameTab)), 0);  // Check content
}

BOOST_AUTO_TEST_CASE(ByteBufferCopy2)
{
   CByteBuffer* buffer = new CByteBuffer(getBuffer());

   // Check buffer content
   unsigned char sameTab[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
   BOOST_CHECK_EQUAL(buffer->size(), sizeof(sameTab));                         // Check size
   BOOST_CHECK_EQUAL(memcmp(buffer->content(), sameTab, sizeof(sameTab)), 0);  // Check content

   CByteBuffer buffer2 = *buffer;
   delete buffer;
   BOOST_CHECK_EQUAL(buffer2.size(), sizeof(sameTab));                         // Check size
   BOOST_CHECK_EQUAL(memcmp(buffer2.content(), sameTab, sizeof(sameTab)), 0);  // Check content
}

BOOST_AUTO_TEST_SUITE_END()
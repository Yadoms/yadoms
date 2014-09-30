#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/communication/FixedSizeReceiveBufferHandler.h"

BOOST_AUTO_TEST_SUITE(TestFixedSizeReceiveBufferHandler)

enum { kEvtId = shared::event::kUserFirstId };

static bool buffersAreEqual(const shared::communication::CByteBuffer& buf1, const shared::communication::CByteBuffer& buf2)
{
   if (buf1.size() != buf2.size())
      return false;

   if (memcmp(buf1.content(), buf2.content(), buf1.size()))
      return false;

   return true;
}

static const shared::communication::CByteBuffer concatBuffers(const shared::communication::CByteBuffer& buf1, const shared::communication::CByteBuffer& buf2)
{
   unsigned char* fullContent = new unsigned char[buf1.size() + buf2.size()];
   memcpy (fullContent, buf1.content(), buf1.size());
   memcpy (fullContent + buf1.size(), buf2.content(), buf2.size());
   const shared::communication::CByteBuffer buf(fullContent, buf1.size() + buf2.size());
   delete[] fullContent;
   return buf;
}

BOOST_AUTO_TEST_CASE(ExactSizeFrom1Part)
{
   unsigned char tab[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
   shared::event::CEventHandler evtHandler;
   shared::communication::CFixedSizeReceiveBufferHandler buffer(evtHandler, kEvtId, sizeof(tab));

   // Push data to buffer
   shared::communication::CByteBuffer sentBuffer(tab, sizeof(tab));
   buffer.push(sentBuffer);

   // EventHandler should be already notified
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), kEvtId);
   BOOST_CHECK_EQUAL(evtHandler.isEventType<const shared::communication::CByteBuffer>(), true);
   shared::communication::CByteBuffer receivedBuffer = evtHandler.getEventData<const shared::communication::CByteBuffer>();
   BOOST_CHECK_EQUAL(buffersAreEqual(sentBuffer, receivedBuffer), true);
}

BOOST_AUTO_TEST_CASE(ExactSizeFrom2Parts)
{
   unsigned char tab1[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
   unsigned char tab2[] = {'1', '2', '3', '4'};
   shared::event::CEventHandler evtHandler;
   shared::communication::CFixedSizeReceiveBufferHandler buffer(evtHandler, kEvtId, sizeof(tab1) + sizeof(tab2));

   // Push first part to buffer
   shared::communication::CByteBuffer sentBuffer1(tab1, sizeof(tab1));
   buffer.push(sentBuffer1);

   // EventHandler should not be notified
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::milliseconds(50)), shared::event::kTimeout);

   // Push second part to buffer
   shared::communication::CByteBuffer sentBuffer2(tab2, sizeof(tab2));
   buffer.push(sentBuffer2);

   // EventHandler should now be notified
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), kEvtId);
   BOOST_CHECK_EQUAL(evtHandler.isEventType<const shared::communication::CByteBuffer>(), true);
   shared::communication::CByteBuffer receivedBuffer = evtHandler.getEventData<const shared::communication::CByteBuffer>();
   BOOST_CHECK_EQUAL(buffersAreEqual(concatBuffers(sentBuffer1, sentBuffer2), receivedBuffer), true);
}

BOOST_AUTO_TEST_CASE(GreaterSizeFrom2Parts)
{
   unsigned char tab1[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
   unsigned char tab2[] = {'1', '2', '3', '4'};
   shared::event::CEventHandler evtHandler;
   shared::communication::CFixedSizeReceiveBufferHandler buffer(evtHandler, kEvtId, sizeof(tab1) + sizeof(tab2) - 2);

   // Push first part to buffer
   shared::communication::CByteBuffer sentBuffer1(tab1, sizeof(tab1));
   buffer.push(sentBuffer1);

   // EventHandler should not be notified
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::milliseconds(50)), shared::event::kTimeout);

   // Push second part to buffer
   shared::communication::CByteBuffer sentBuffer2(tab2, sizeof(tab2));
   buffer.push(sentBuffer2);

   // Expected buffer = sentBuffer1 + sentBuffer2 - 2 bytes
   shared::communication::CByteBuffer expectedBuffer(concatBuffers(sentBuffer1, shared::communication::CByteBuffer(tab2, sizeof(tab2) - 2)));

   // EventHandler should now be notified
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), kEvtId);
   BOOST_CHECK_EQUAL(evtHandler.isEventType<const shared::communication::CByteBuffer>(), true);
   shared::communication::CByteBuffer receivedBuffer = evtHandler.getEventData<const shared::communication::CByteBuffer>();
   BOOST_CHECK_EQUAL(buffersAreEqual(expectedBuffer, receivedBuffer), true);
}

BOOST_AUTO_TEST_SUITE_END()
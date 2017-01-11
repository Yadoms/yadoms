#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/communication/EOFReceiveBufferHandler.h"

BOOST_AUTO_TEST_SUITE(TestEOFReceiveBufferHandler)

enum { kEvtId = shared::event::kUserFirstId };

static bool buffersAreEqual(const shared::communication::CByteBuffer& buf1, const shared::communication::CByteBuffer& buf2)
{
   if (buf1.size() != buf2.size())
      return false;

   if (memcmp(buf1.begin(), buf2.begin(), buf1.size()))
      return false;

   return true;
}

BOOST_AUTO_TEST_CASE(OneMessage)
{
   enum { EOF_CHAR = 0x03 };
   unsigned char tab[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', EOF_CHAR};
   shared::event::CEventHandler evtHandler;
   shared::communication::CEOFReceiveBufferHandler buffer(evtHandler, kEvtId, EOF_CHAR);

   // Push data to buffer
   shared::communication::CByteBuffer sentBuffer(tab, sizeof(tab));
   buffer.push(sentBuffer);

   // EventHandler should be already notified
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), kEvtId);
   BOOST_CHECK_EQUAL(evtHandler.isEventType<const shared::communication::CByteBuffer>(), true);
   shared::communication::CByteBuffer receivedBuffer = evtHandler.getEventData<const shared::communication::CByteBuffer>();
   BOOST_CHECK_EQUAL(buffersAreEqual(sentBuffer, receivedBuffer), true);
}

BOOST_AUTO_TEST_CASE(TwoMessages)
{
   enum { EOF_CHAR = 'Z' };
   unsigned char tab    [] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', EOF_CHAR, '1', '2', '3', '4', EOF_CHAR};
   unsigned char subTab1[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', EOF_CHAR                              };
   unsigned char subTab2[] = {                                             '1', '2', '3', '4', EOF_CHAR};
   shared::event::CEventHandler evtHandler;
   shared::communication::CEOFReceiveBufferHandler buffer(evtHandler, kEvtId, EOF_CHAR);

   // Push data to buffer
   buffer.push(shared::communication::CByteBuffer(tab, sizeof(tab)));

   // EventHandler should be already notified ==> First message
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), kEvtId);
   BOOST_CHECK_EQUAL(evtHandler.isEventType<const shared::communication::CByteBuffer>(), true);
   shared::communication::CByteBuffer receivedBuffer = evtHandler.getEventData<const shared::communication::CByteBuffer>();
   BOOST_CHECK_EQUAL(buffersAreEqual(shared::communication::CByteBuffer(subTab1, sizeof(subTab1)), receivedBuffer), true);

   // EventHandler should be already notified ==> Second message
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), kEvtId);
   BOOST_CHECK_EQUAL(evtHandler.isEventType<const shared::communication::CByteBuffer>(), true);
   receivedBuffer = evtHandler.getEventData<const shared::communication::CByteBuffer>();
   BOOST_CHECK_EQUAL(buffersAreEqual(shared::communication::CByteBuffer(subTab2, sizeof(subTab2)), receivedBuffer), true);
}

BOOST_AUTO_TEST_CASE(TwoMessagesWithSecondMessageCut)
{
   enum { EOF_CHAR = ' ' };
   unsigned char sentTab1    [] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', EOF_CHAR, '1', '2'                    };
   unsigned char sentTab2    [] = {                                                       '3', '4', EOF_CHAR};
   unsigned char expectedTab1[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', EOF_CHAR                              };
   unsigned char expectedTab2[] = {                                             '1', '2', '3', '4', EOF_CHAR};

   shared::event::CEventHandler evtHandler;
   shared::communication::CEOFReceiveBufferHandler buffer(evtHandler, kEvtId, EOF_CHAR);

   // Push data to buffer, first part
   buffer.push(shared::communication::CByteBuffer(sentTab1, sizeof(sentTab1)));

   // EventHandler should be already notified ==> First message
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), kEvtId);
   BOOST_CHECK_EQUAL(evtHandler.isEventType<const shared::communication::CByteBuffer>(), true);
   auto receivedBuffer = evtHandler.getEventData<const shared::communication::CByteBuffer>();
   BOOST_CHECK_EQUAL(buffersAreEqual(shared::communication::CByteBuffer(expectedTab1, sizeof(expectedTab1)), receivedBuffer), true);

   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::milliseconds(50)), shared::event::kTimeout);

   // Push data to buffer, second part
   buffer.push(shared::communication::CByteBuffer(sentTab2, sizeof(sentTab2)));

   // EventHandler should be already notified ==> Second message
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), kEvtId);
   BOOST_CHECK_EQUAL(evtHandler.isEventType<const shared::communication::CByteBuffer>(), true);
   receivedBuffer = evtHandler.getEventData<const shared::communication::CByteBuffer>();
   BOOST_CHECK_EQUAL(buffersAreEqual(shared::communication::CByteBuffer(expectedTab2, sizeof(expectedTab2)), receivedBuffer), true);
}

BOOST_AUTO_TEST_CASE(Flush)
{
   enum { EOF_CHAR = ' ' };
   unsigned char sentTab1[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', '0', '1', '2' };
   unsigned char sentTab2[] = {                                                  '3', '4', EOF_CHAR };
   unsigned char expectedTab[] = { '3', '4', EOF_CHAR };

   shared::event::CEventHandler evtHandler;
   shared::communication::CEOFReceiveBufferHandler buffer(evtHandler, kEvtId, EOF_CHAR);

   // Push data to buffer, first part
   buffer.push(shared::communication::CByteBuffer(sentTab1, sizeof(sentTab1)));

   // EventHandler should not have been notified
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::milliseconds(50)), shared::event::kTimeout);

   // Flush buffer
   buffer.flush();

   // Push data to buffer, second part
   buffer.push(shared::communication::CByteBuffer(sentTab2, sizeof(sentTab2)));

   // EventHandler should have been notified
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), kEvtId);
   BOOST_CHECK_EQUAL(evtHandler.isEventType<const shared::communication::CByteBuffer>(), true);
   auto receivedBuffer = evtHandler.getEventData<const shared::communication::CByteBuffer>();
   BOOST_CHECK_EQUAL(buffersAreEqual(shared::communication::CByteBuffer(expectedTab, sizeof(expectedTab)), receivedBuffer), true);
}

BOOST_AUTO_TEST_SUITE_END()
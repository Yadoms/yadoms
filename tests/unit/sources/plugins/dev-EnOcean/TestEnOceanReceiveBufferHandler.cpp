#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/dev-EnOcean/EnOceanReceiveBufferHandler.h"
#include <../../../../sources/shared/shared/communication/Buffer.hpp>

// Includes needed to compile the test
#include "../../testCommon/fileSystem.h"


BOOST_AUTO_TEST_SUITE(TestEnOceanReceiveBufferHandler)

BOOST_AUTO_TEST_CASE(SmallestMessage)
{
   std::vector<unsigned char> message {
      0x55, 0x00, 0x00, 0x00, EnOceanMessage::RADIO_ERP1,         // Header
      0x07,                                                       // CRC8H
      0x00                                                        // CRC8D
   };

   shared::event::CEventHandler evtHandler;
   CEnOceanReceiveBufferHandler handler(evtHandler, shared::event::kUserFirstId);
   handler.push(shared::communication::CByteBuffer(&message[0], message.size()));

   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);

   auto data = evtHandler.getEventData<const EnOceanMessage::CMessage>();
   BOOST_CHECK_EQUAL(data.header().dataLength(), 0);
   BOOST_CHECK_EQUAL(data.header().optionalLength(), 0);
   BOOST_CHECK_EQUAL(data.header().packetType(), EnOceanMessage::RADIO_ERP1);
   BOOST_CHECK_EQUAL(data.header().offsetData(), 6);
   BOOST_CHECK_EQUAL(data.header().offsetOptional(), 6);

   std::vector<unsigned char> expectedData{};
   BOOST_CHECK_EQUAL_COLLECTIONS(data.data().begin(), data.data().end(),
      expectedData.begin(), expectedData.end());

   BOOST_CHECK_EQUAL(data.optional().size(), static_cast<size_t>(0));
}

BOOST_AUTO_TEST_CASE(MessageWithData)
{
   std::vector<unsigned char> message {
      0x55, 0x00, 0x08, 0x00, EnOceanMessage::RADIO_ERP1,         // Header
      0x56,                                                       // CRC8H
      0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,             // Data
      0x3E                                                        // CRC8D
   };

   shared::event::CEventHandler evtHandler;
   CEnOceanReceiveBufferHandler handler(evtHandler, shared::event::kUserFirstId);
   handler.push(shared::communication::CByteBuffer(&message[0], message.size()));

   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);

   auto data = evtHandler.getEventData<const EnOceanMessage::CMessage>();
   BOOST_CHECK_EQUAL(data.header().dataLength(), 8);
   BOOST_CHECK_EQUAL(data.header().optionalLength(), 0);
   BOOST_CHECK_EQUAL(data.header().packetType(), EnOceanMessage::RADIO_ERP1);
   BOOST_CHECK_EQUAL(data.header().offsetData(), 6);
   BOOST_CHECK_EQUAL(data.header().offsetOptional(), 6 + 8);

   std::vector<unsigned char> expectedData{ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
   BOOST_CHECK_EQUAL_COLLECTIONS(data.data().begin(), data.data().end(),
      expectedData.begin(), expectedData.end());

   BOOST_CHECK_EQUAL(data.optional().size(), static_cast<size_t>(0));
}

BOOST_AUTO_TEST_CASE(MessageWithDataAndOptional)
{
   std::vector<unsigned char> message{
      0x55, 0x00, 0x08, 0x07, EnOceanMessage::RADIO_ERP1,         // Header
      0x3D,                                                       // CRC8H
      0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,             // Data
      0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,                   // Optional
      0x41                                                        // CRC8D
   };

   shared::event::CEventHandler evtHandler;
   CEnOceanReceiveBufferHandler handler(evtHandler, shared::event::kUserFirstId);
   handler.push(shared::communication::CByteBuffer(&message[0], message.size()));

   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);

   auto data = evtHandler.getEventData<const EnOceanMessage::CMessage>();
   BOOST_CHECK_EQUAL(data.header().dataLength(), 8);
   BOOST_CHECK_EQUAL(data.header().optionalLength(), 7);
   BOOST_CHECK_EQUAL(data.header().packetType(), EnOceanMessage::RADIO_ERP1);
   BOOST_CHECK_EQUAL(data.header().offsetData(), 6);
   BOOST_CHECK_EQUAL(data.header().offsetOptional(), 6 + 8);

   std::vector<unsigned char> expectedData{ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
   BOOST_CHECK_EQUAL_COLLECTIONS(data.data().begin(), data.data().end(),
      expectedData.begin(), expectedData.end());

   std::vector<unsigned char> expectedOptional{ 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
   BOOST_CHECK_EQUAL_COLLECTIONS(data.optional().begin(), data.optional().end(),
      expectedOptional.begin(), expectedOptional.end());
}

//TODO  : test avec sync byte pas au début du message
//TODO  : test sur timeout

BOOST_AUTO_TEST_SUITE_END()
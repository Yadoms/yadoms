#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/EnOcean/ReceiveBufferHandler.h"
#include "../../../../sources/plugins/EnOcean/message/DongleVersionResponseReceivedMessage.h"
#include <../../../../sources/shared/shared/communication/Buffer.hpp>

#include "MessageHandlerMock.hpp"


BOOST_AUTO_TEST_SUITE(TestDongleVersionResponseReceivedMessage)

   BOOST_AUTO_TEST_CASE(TestNominal)
   {
      std::vector<unsigned char> message{
         0x55, 0x00, 0x21, 0x00, 0x02, 0x26, 0x00, 0x02, 0x0b, 0x01, 0x00, 0x02, 0x06, 0x03, 0x00, 0x01,
         0x9f, 0x7a, 0x59, 0x45, 0x4f, 0x01, 0x03, 0x47, 0x41, 0x54, 0x45, 0x57, 0x41, 0x59, 0x43, 0x54,
         0x52, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c
      };

      shared::event::CEventHandler evtHandler;
      const auto msgHandler = boost::make_shared<CMessageHandlerMock>(evtHandler, shared::event::kUserFirstId);
      CReceiveBufferHandler bufferHandler(msgHandler);
      bufferHandler.push(shared::communication::CByteBuffer(&message[0], message.size()));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);

      auto data = evtHandler.getEventData<boost::shared_ptr<const message::CEsp3ReceivedPacket>>();
      BOOST_CHECK_EQUAL(data->header().dataLength(), 33);
      BOOST_CHECK_EQUAL(data->header().optionalLength(), 0);
      BOOST_CHECK_EQUAL(data->header().packetType(), message::RESPONSE);
      BOOST_CHECK_EQUAL(data->header().offsetData(), 6);
      BOOST_CHECK_EQUAL(data->header().offsetOptional(), 33 + 6);

      const auto& response = boost::make_shared<message::CResponseReceivedMessage>(data);
      BOOST_CHECK_EQUAL(response->returnCode(), message::CResponseReceivedMessage::RET_OK);
      BOOST_CHECK_EQUAL(response->responseData().size(), static_cast<size_t>(33 - 1));

      const auto& dongleVersionMessage = message::CDongleVersionResponseReceivedMessage(response);
      BOOST_CHECK_EQUAL(dongleVersionMessage.chipId(), "019F7A59");
      BOOST_CHECK_EQUAL(dongleVersionMessage.fullVersion(), "EnOcean dongle Version 2.11.1.0 GATEWAYCTRL, api 2.6.3.0, chipId 019F7A59, chipVersion 454f0103");
}

BOOST_AUTO_TEST_SUITE_END()


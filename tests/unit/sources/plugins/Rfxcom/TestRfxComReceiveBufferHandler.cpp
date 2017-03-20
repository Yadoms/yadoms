#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/Rfxcom/RfxComReceiveBufferHandler.h"
#include <../../../../sources/shared/shared/communication/Buffer.hpp>

#include "../../mock/shared/currentTime/DefaultCurrentTimeMock.h"

// Includes needed to compile the test
#include "../../testCommon/fileSystem.h"


BOOST_AUTO_TEST_SUITE(TestRfxComReceiveBufferHandler)

   BOOST_AUTO_TEST_CASE(NominalMessage)
   {
      std::vector<unsigned char> txMessage{
         0x14, 0x00, 0x00, 0x00, 0x00, // packetlength, packettype, subtype, seqnbr, cmnd
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // msg[1-8]
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // msg[9-16]
      };

      shared::event::CEventHandler evtHandler;
      CRfxcomReceiveBufferHandler bufferHandler(evtHandler, shared::event::kUserFirstId);
      bufferHandler.push(shared::communication::CByteBuffer(txMessage));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId) ;

      auto rxMessage = evtHandler.getEventData<const shared::communication::CByteBuffer>();
      BOOST_CHECK_EQUAL(rxMessage.size(), txMessage.size()) ;

      BOOST_CHECK_EQUAL_COLLECTIONS(rxMessage.begin(), rxMessage.end(),
         txMessage.begin(), txMessage.end()) ;

      BOOST_CHECK_EQUAL(rxMessage[0], rxMessage.size()-1) ;
   }

   BOOST_AUTO_TEST_CASE(UncompletedMessage)
   {
      std::vector<unsigned char> txMessage1{
         0x14, 0x00, 0x00, 0x00, 0x00, // packetlength, packettype, subtype, seqnbr, cmnd
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // msg[1-8]
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // msg[9-16] ==> 1 byte missing
      };

      std::vector<unsigned char> txMessage2{
         0x14, 0x00, 0x00, 0x00, 0x00, // packetlength, packettype, subtype, seqnbr, cmnd
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // msg[1-8]
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // msg[9-16]
      };

      auto timeProviderMock = boost::make_shared<CDefaultCurrentTimeMock>();
      shared::currentTime::Provider().setProvider(timeProviderMock);

      shared::event::CEventHandler evtHandler;
      CRfxcomReceiveBufferHandler bufferHandler(evtHandler, shared::event::kUserFirstId);
      bufferHandler.push(shared::communication::CByteBuffer(txMessage1));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent) ;

      timeProviderMock->sleep(boost::posix_time::seconds(1));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent) ;

      bufferHandler.push(shared::communication::CByteBuffer(txMessage2));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId) ;

      auto rxMessage = evtHandler.getEventData<const shared::communication::CByteBuffer>();
      BOOST_CHECK_EQUAL(rxMessage.size(), txMessage2.size()) ;
      BOOST_CHECK_EQUAL(rxMessage[0], rxMessage.size() - 1);

      BOOST_CHECK_EQUAL_COLLECTIONS(rxMessage.begin(), rxMessage.end(),
         txMessage2.begin(), txMessage2.end()) ;
   }

   BOOST_AUTO_TEST_CASE(AggregatedMessages)
   {
      std::vector<unsigned char> txMessage1{
         0x14, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
      };
      std::vector<unsigned char> txMessage2{
         0x0B, 0x15, 0x00, 0x0E, 0x01, 0x0B,
         0x15, 0x00, 0x01, 0x0B, 0x15, 0x0B
      };
      std::vector<unsigned char> txMessage3{
         0x05, 0x00, 0x00, 0x00, 0x00, 0x00
      };
      std::vector<unsigned char> txMessage;
      txMessage.insert(txMessage.end(), txMessage1.begin(), txMessage1.end());
      txMessage.insert(txMessage.end(), txMessage2.begin(), txMessage2.end());
      txMessage.insert(txMessage.end(), txMessage3.begin(), txMessage3.end());

      shared::event::CEventHandler evtHandler;
      CRfxcomReceiveBufferHandler bufferHandler(evtHandler, shared::event::kUserFirstId);
      bufferHandler.push(shared::communication::CByteBuffer(txMessage));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      auto rxMessage1 = evtHandler.getEventData<const shared::communication::CByteBuffer>();
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      auto rxMessage2 = evtHandler.getEventData<const shared::communication::CByteBuffer>();
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId);
      auto rxMessage3 = evtHandler.getEventData<const shared::communication::CByteBuffer>();

      BOOST_CHECK_EQUAL(rxMessage1[0], rxMessage1.size() - 1);
      BOOST_CHECK_EQUAL_COLLECTIONS(rxMessage1.begin(), rxMessage1.end(),
         txMessage1.begin(), txMessage1.end());

      BOOST_CHECK_EQUAL(rxMessage2[0], rxMessage2.size() - 1);
      BOOST_CHECK_EQUAL_COLLECTIONS(rxMessage2.begin(), rxMessage2.end(),
         txMessage2.begin(), txMessage2.end());

      BOOST_CHECK_EQUAL(rxMessage3[0], rxMessage3.size() - 1);
      BOOST_CHECK_EQUAL_COLLECTIONS(rxMessage3.begin(), rxMessage3.end(),
         txMessage3.begin(), txMessage3.end());
   }

   BOOST_AUTO_TEST_SUITE_END()


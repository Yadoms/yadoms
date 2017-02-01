#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/EnOcean/ReceiveBufferHandler.h"
#include "../../../../sources/plugins/EnOcean/profiles/generated-eep.h"
#include <../../../../sources/shared/shared/communication/Buffer.hpp>

#include "../../mock/shared/currentTime/DefaultCurrentTimeMock.h"

// Includes needed to compile the test
#include "../../testCommon/fileSystem.h"


// Mock
CRorgs::ERorgIds CRorgs::toRorgId(unsigned int id)
{
   return k4BS_Telegram;
}

class CMessageHandlerMock : public IMessageHandler
{
public:
   explicit CMessageHandlerMock(shared::event::CEventHandler& evtHandler, int evtId)
      :m_evtHandler(evtHandler), m_evtId(evtId)
   {
   }
   void send(message::CEsp3SendPacket& sendMessage) override
   {
   }
   bool send(message::CEsp3SendPacket& sendMessage,
                     boost::function<bool(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> isExpectedMessageFct,
                     boost::function<void(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> onReceiveFct) override
   {
      return false;
   }
   void onReceived(boost::shared_ptr<const message::CEsp3ReceivedPacket> receivedMessage) override
   {
      m_evtHandler.postEvent(m_evtId, receivedMessage);
   }
private:
   shared::event::CEventHandler& m_evtHandler;
   int m_evtId;
};


BOOST_AUTO_TEST_SUITE(TestEnOceanReceiveBufferHandler)

   BOOST_AUTO_TEST_CASE(SmallestMessage)
   {
      std::vector<unsigned char> message{
         0x55, 0x00, 0x00, 0x00, message::RADIO_ERP1, // Header
         0x07, // CRC8H
         0x00 // CRC8D
      };

      shared::event::CEventHandler evtHandler;
      auto msgHandler = boost::make_shared<CMessageHandlerMock>(evtHandler, shared::event::kUserFirstId);
      CReceiveBufferHandler bufferHandler(msgHandler);
      bufferHandler.push(shared::communication::CByteBuffer(&message[0], message.size()));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId) ;

      auto data = evtHandler.getEventData<boost::shared_ptr<const message::CEsp3ReceivedPacket>>();
      BOOST_CHECK_EQUAL(data->header().dataLength(), 0) ;
      BOOST_CHECK_EQUAL(data->header().optionalLength(), 0) ;
      BOOST_CHECK_EQUAL(data->header().packetType(), message::RADIO_ERP1) ;
      BOOST_CHECK_EQUAL(data->header().offsetData(), 6) ;
      BOOST_CHECK_EQUAL(data->header().offsetOptional(), 6) ;

      std::vector<unsigned char> expectedData{};
      BOOST_CHECK_EQUAL_COLLECTIONS(data->data().begin(), data->data().end(),
         expectedData.begin(), expectedData.end()) ;

      BOOST_CHECK_EQUAL(data->optional().size(), static_cast<size_t>(0)) ;
   }

   BOOST_AUTO_TEST_CASE(MessageWithData)
   {
      std::vector<unsigned char> message{
         0x55, 0x00, 0x08, 0x00, message::RADIO_ERP1, // Header
         0x56, // CRC8H
         0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, // Data
         0x3E // CRC8D
      };

      shared::event::CEventHandler evtHandler;
      auto msgHandler = boost::make_shared<CMessageHandlerMock>(evtHandler, shared::event::kUserFirstId);
      CReceiveBufferHandler bufferHandler(msgHandler);
      bufferHandler.push(shared::communication::CByteBuffer(&message[0], message.size()));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId) ;

      auto data = evtHandler.getEventData<boost::shared_ptr<const message::CEsp3ReceivedPacket>>();
      BOOST_CHECK_EQUAL(data->header().dataLength(), 8) ;
      BOOST_CHECK_EQUAL(data->header().optionalLength(), 0) ;
      BOOST_CHECK_EQUAL(data->header().packetType(), message::RADIO_ERP1) ;
      BOOST_CHECK_EQUAL(data->header().offsetData(), 6) ;
      BOOST_CHECK_EQUAL(data->header().offsetOptional(), 6 + 8) ;

      std::vector<unsigned char> expectedData{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
      BOOST_CHECK_EQUAL_COLLECTIONS(data->data().begin(), data->data().end(),
         expectedData.begin(), expectedData.end()) ;

      BOOST_CHECK_EQUAL(data->optional().size(), static_cast<size_t>(0)) ;
   }

   BOOST_AUTO_TEST_CASE(MessageWithDataAndOptional)
   {
      std::vector<unsigned char> message{
         0x55, 0x00, 0x08, 0x07, message::RADIO_ERP1, // Header
         0x3D, // CRC8H
         0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, // Data
         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, // Optional
         0x41 // CRC8D
      };

      shared::event::CEventHandler evtHandler;
      auto msgHandler = boost::make_shared<CMessageHandlerMock>(evtHandler, shared::event::kUserFirstId);
      CReceiveBufferHandler bufferHandler(msgHandler);
      bufferHandler.push(shared::communication::CByteBuffer(&message[0], message.size()));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId) ;

      auto data = evtHandler.getEventData<boost::shared_ptr<const message::CEsp3ReceivedPacket>>();
      BOOST_CHECK_EQUAL(data->header().dataLength(), 8) ;
      BOOST_CHECK_EQUAL(data->header().optionalLength(), 7) ;
      BOOST_CHECK_EQUAL(data->header().packetType(), message::RADIO_ERP1) ;
      BOOST_CHECK_EQUAL(data->header().offsetData(), 6) ;
      BOOST_CHECK_EQUAL(data->header().offsetOptional(), 6 + 8) ;

      std::vector<unsigned char> expectedData{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
      BOOST_CHECK_EQUAL_COLLECTIONS(data->data().begin(), data->data().end(),
         expectedData.begin(), expectedData.end()) ;

      std::vector<unsigned char> expectedOptional{0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
      BOOST_CHECK_EQUAL_COLLECTIONS(data->optional().begin(), data->optional().end(),
         expectedOptional.begin(), expectedOptional.end()) ;
   }

   BOOST_AUTO_TEST_CASE(MessageWrongCrc8h)
   {
      std::vector<unsigned char> message{
         0x55, 0x00, 0x08, 0x07, message::RADIO_ERP1, // Header
         0x3E, // CRC8H (wrong. Correct value is 0x3D)
         0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, // Data
         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, // Optional
         0x41 // CRC8D
      };

      shared::event::CEventHandler evtHandler;
      auto msgHandler = boost::make_shared<CMessageHandlerMock>(evtHandler, shared::event::kUserFirstId);
      CReceiveBufferHandler bufferHandler(msgHandler);
      bufferHandler.push(shared::communication::CByteBuffer(&message[0], message.size()));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent) ;
   }

   BOOST_AUTO_TEST_CASE(MessageWrongCrc8d)
   {
      std::vector<unsigned char> message{
         0x55, 0x00, 0x08, 0x07, message::RADIO_ERP1, // Header
         0x3D, // CRC8H
         0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, // Data
         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, // Optional
         0x42 // CRC8D (wrong. Correct value is 0x41)
      };

      shared::event::CEventHandler evtHandler;
      auto msgHandler = boost::make_shared<CMessageHandlerMock>(evtHandler, shared::event::kUserFirstId);
      CReceiveBufferHandler bufferHandler(msgHandler);
      bufferHandler.push(shared::communication::CByteBuffer(&message[0], message.size()));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent) ;
   }

   BOOST_AUTO_TEST_CASE(MessageWithParasiticPrefix)
   {
      std::vector<unsigned char> message{
         0x12, 0x23, // Parasitic prefix, should be filtered
         0x55, 0x00, 0x08, 0x07, message::RADIO_ERP1, // Header
         0x3D, // CRC8H
         0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, // Data
         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, // Optional
         0x41 // CRC8D
      };

      shared::event::CEventHandler evtHandler;
      auto msgHandler = boost::make_shared<CMessageHandlerMock>(evtHandler, shared::event::kUserFirstId);
      CReceiveBufferHandler bufferHandler(msgHandler);
      bufferHandler.push(shared::communication::CByteBuffer(&message[0], message.size()));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId) ;

      auto data = evtHandler.getEventData<boost::shared_ptr<const message::CEsp3ReceivedPacket>>();
      BOOST_CHECK_EQUAL(data->header().dataLength(), 8) ;
      BOOST_CHECK_EQUAL(data->header().optionalLength(), 7) ;
      BOOST_CHECK_EQUAL(data->header().packetType(), message::RADIO_ERP1) ;
      BOOST_CHECK_EQUAL(data->header().offsetData(), 6) ;
      BOOST_CHECK_EQUAL(data->header().offsetOptional(), 6 + 8) ;

      std::vector<unsigned char> expectedData{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
      BOOST_CHECK_EQUAL_COLLECTIONS(data->data().begin(), data->data().end(),
         expectedData.begin(), expectedData.end()) ;

      std::vector<unsigned char> expectedOptional{0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
      BOOST_CHECK_EQUAL_COLLECTIONS(data->optional().begin(), data->optional().end(),
         expectedOptional.begin(), expectedOptional.end()) ;
   }

   BOOST_AUTO_TEST_CASE(MessageWithoutSync)
   {
      std::vector<unsigned char> message{
         0x50, 0x00, 0x08, 0x07, message::RADIO_ERP1, // Header (no sync byte)
         0x3D, // CRC8H
         0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, // Data
         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, // Optional
         0x41 // CRC8D
      };

      shared::event::CEventHandler evtHandler;
      auto msgHandler = boost::make_shared<CMessageHandlerMock>(evtHandler, shared::event::kUserFirstId);
      CReceiveBufferHandler bufferHandler(msgHandler);
      bufferHandler.push(shared::communication::CByteBuffer(&message[0], message.size()));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent) ;
   }

   BOOST_AUTO_TEST_CASE(MessageWithParasiticPrefixContainingSync)
   {
      std::vector<unsigned char> message{
         // Wrong data (invalid message : CRC8H KO), should be filtered
         0x55, 0x00, 0x08, 0x07, message::RADIO_ERP1, // Header (no sync byte)
         0x3E, // CRC8H (should be 0x3D)
         0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, // Data
         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, // Optional
         0x41, // CRC8D

         // Correct message
         0x55, 0x00, 0x08, 0x07, message::RADIO_ERP1, // Header
         0x3D, // CRC8H
         0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, // Data
         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, // Optional
         0x41 // CRC8D
      };

      shared::event::CEventHandler evtHandler;
      auto msgHandler = boost::make_shared<CMessageHandlerMock>(evtHandler, shared::event::kUserFirstId);
      CReceiveBufferHandler bufferHandler(msgHandler);
      bufferHandler.push(shared::communication::CByteBuffer(&message[0], message.size()));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId) ;

      auto data = evtHandler.getEventData<boost::shared_ptr<const message::CEsp3ReceivedPacket>>();
      BOOST_CHECK_EQUAL(data->header().dataLength(), 8) ;
      BOOST_CHECK_EQUAL(data->header().optionalLength(), 7) ;
      BOOST_CHECK_EQUAL(data->header().packetType(), message::RADIO_ERP1) ;
      BOOST_CHECK_EQUAL(data->header().offsetData(), 6) ;
      BOOST_CHECK_EQUAL(data->header().offsetOptional(), 6 + 8) ;

      std::vector<unsigned char> expectedData{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
      BOOST_CHECK_EQUAL_COLLECTIONS(data->data().begin(), data->data().end(),
         expectedData.begin(), expectedData.end()) ;

      std::vector<unsigned char> expectedOptional{0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
      BOOST_CHECK_EQUAL_COLLECTIONS(data->optional().begin(), data->optional().end(),
         expectedOptional.begin(), expectedOptional.end()) ;
   }

   BOOST_AUTO_TEST_CASE(MessageSendInSeveralParts)
   {
      std::vector<unsigned char> message1{
         0x55, 0x00, 0x08, 0x07, message::RADIO_ERP1, // Header
      };

      std::vector<unsigned char> message2{
         0x3D, // CRC8H
         0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, // Data
         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, // Optional
         0x41 // CRC8D
      };

      shared::event::CEventHandler evtHandler;
      auto msgHandler = boost::make_shared<CMessageHandlerMock>(evtHandler, shared::event::kUserFirstId);
      CReceiveBufferHandler bufferHandler(msgHandler);
      bufferHandler.push(shared::communication::CByteBuffer(&message1[0], message1.size()));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent) ;

      bufferHandler.push(shared::communication::CByteBuffer(&message2[0], message2.size()));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId) ;

      auto data = evtHandler.getEventData<boost::shared_ptr<const message::CEsp3ReceivedPacket>>();
      BOOST_CHECK_EQUAL(data->header().dataLength(), 8) ;
      BOOST_CHECK_EQUAL(data->header().optionalLength(), 7) ;
      BOOST_CHECK_EQUAL(data->header().packetType(), message::RADIO_ERP1) ;
      BOOST_CHECK_EQUAL(data->header().offsetData(), 6) ;
      BOOST_CHECK_EQUAL(data->header().offsetOptional(), 6 + 8) ;

      std::vector<unsigned char> expectedData{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
      BOOST_CHECK_EQUAL_COLLECTIONS(data->data().begin(), data->data().end(),
         expectedData.begin(), expectedData.end()) ;

      std::vector<unsigned char> expectedOptional{0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
      BOOST_CHECK_EQUAL_COLLECTIONS(data->optional().begin(), data->optional().end(),
         expectedOptional.begin(), expectedOptional.end()) ;
   }

   BOOST_AUTO_TEST_CASE(Timeout)
   {
      std::vector<unsigned char> message1{
         0x55, 0x00, 0x08, 0x07, message::RADIO_ERP1, // Header
         0x3D, // CRC8H
         0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, // Data
         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, // Optional
         // No CRC8D (missing byte)
      };

      std::vector<unsigned char> message2{
         0x41 // CRC8D
      };

      auto timeProviderMock = boost::make_shared<CDefaultCurrentTimeMock>();
      shared::currentTime::Provider().setProvider(timeProviderMock);

      shared::event::CEventHandler evtHandler;
      auto msgHandler = boost::make_shared<CMessageHandlerMock>(evtHandler, shared::event::kUserFirstId);
      CReceiveBufferHandler bufferHandler(msgHandler);
      bufferHandler.push(shared::communication::CByteBuffer(&message1[0], message1.size()));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent) ;

      timeProviderMock->sleep(boost::posix_time::milliseconds(101)); // Timeout should be 100ms

      bufferHandler.push(shared::communication::CByteBuffer(&message2[0], message2.size()));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent) ;
   }

   BOOST_AUTO_TEST_CASE(JustBeforeTimeout)
   {
      std::vector<unsigned char> message1{
         0x55, 0x00, 0x08, 0x07, message::RADIO_ERP1, // Header
         0x3D, // CRC8H
         0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, // Data
         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, // Optional
         // No CRC8D (missing byte)
      };

      std::vector<unsigned char> message2{
         0x41 // CRC8D
      };

      auto timeProviderMock = boost::make_shared<CDefaultCurrentTimeMock>();
      shared::currentTime::Provider().setProvider(timeProviderMock);

      shared::event::CEventHandler evtHandler;
      auto msgHandler = boost::make_shared<CMessageHandlerMock>(evtHandler, shared::event::kUserFirstId);
      CReceiveBufferHandler bufferHandler(msgHandler);
      bufferHandler.push(shared::communication::CByteBuffer(&message1[0], message1.size()));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent) ;

      timeProviderMock->sleep(boost::posix_time::milliseconds(99)); // Timeout should be 100ms

      bufferHandler.push(shared::communication::CByteBuffer(&message2[0], message2.size()));

      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId) ;

      auto data = evtHandler.getEventData<boost::shared_ptr<const message::CEsp3ReceivedPacket>>();
      BOOST_CHECK_EQUAL(data->header().dataLength(), 8) ;
      BOOST_CHECK_EQUAL(data->header().optionalLength(), 7) ;
      BOOST_CHECK_EQUAL(data->header().packetType(), message::RADIO_ERP1) ;
      BOOST_CHECK_EQUAL(data->header().offsetData(), 6) ;
      BOOST_CHECK_EQUAL(data->header().offsetOptional(), 6 + 8) ;

      std::vector<unsigned char> expectedData{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
      BOOST_CHECK_EQUAL_COLLECTIONS(data->data().begin(), data->data().end(),
         expectedData.begin(), expectedData.end()) ;

      std::vector<unsigned char> expectedOptional{0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
      BOOST_CHECK_EQUAL_COLLECTIONS(data->optional().begin(), data->optional().end(),
         expectedOptional.begin(), expectedOptional.end()) ;
   }

   BOOST_AUTO_TEST_SUITE_END()


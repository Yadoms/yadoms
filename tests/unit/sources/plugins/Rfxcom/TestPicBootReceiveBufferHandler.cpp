#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/Rfxcom/PicBootPacketControlCharacters.h"
#include "../../../../sources/plugins/Rfxcom/PicBootReceiveBufferHandler.h"
#include <../../../../sources/shared/shared/communication/Buffer.hpp>
#include <../../../../sources/shared/shared/communication/NoBufferLogger.h>

#include "../../mock/shared/currentTime/DefaultCurrentTimeMock.h"

static shared::communication::CNoBufferLogger noLogger;


void checkFilteredMessage(const std::vector<unsigned char>& txMessage)
{
   auto timeProviderMock = useTimeMock();
   shared::event::CEventHandler evtHandler;
   CPicBootReceiveBufferHandler bufferHandler(evtHandler, shared::event::kUserFirstId, boost::posix_time::seconds(1), noLogger);
   bufferHandler.push(shared::communication::CByteBuffer(txMessage));

   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent) ;
   timeProviderMock->sleep(boost::posix_time::seconds(2));
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent) ;
}

void checkExpectedMessage(const std::vector<unsigned char>& txMessage,
                          const std::vector<unsigned char>& expectedMessage)
{
   shared::event::CEventHandler evtHandler;
   CPicBootReceiveBufferHandler bufferHandler(evtHandler, shared::event::kUserFirstId, boost::posix_time::seconds(1), noLogger);
   bufferHandler.push(shared::communication::CByteBuffer(txMessage));

   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId) ;

   auto rxMessage = evtHandler.getEventData<const std::vector<unsigned char>>();
   BOOST_CHECK_EQUAL_COLLECTIONS(
      rxMessage.begin(), rxMessage.end(),
      expectedMessage.begin(), expectedMessage.end()) ;
}

void checkExpectedMessages(const std::vector<unsigned char>& txMessage,
                           const std::vector<std::vector<unsigned char>>& expectedMessages)
{
   shared::event::CEventHandler evtHandler;
   CPicBootReceiveBufferHandler bufferHandler(evtHandler, shared::event::kUserFirstId, boost::posix_time::seconds(1), noLogger);
   bufferHandler.push(shared::communication::CByteBuffer(txMessage));

   for (const auto& expectedMessage:expectedMessages)
   {
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId) ;

      auto rxMessage = evtHandler.getEventData<const std::vector<unsigned char>>();
      BOOST_CHECK_EQUAL_COLLECTIONS(
         rxMessage.begin(), rxMessage.end(),
         expectedMessage.begin(), expectedMessage.end()) ;
   }
}

BOOST_AUTO_TEST_SUITE(TestPicBootReceiveBufferHandler)

   BOOST_AUTO_TEST_CASE(NominalMessage)
   {
      std::vector<unsigned char> txMessage{
         kSTX, kSTX, 0x00, 0x00, 0x00, kETX // STX, STX, Data0, Data1, checksum, ETX
      };
      std::vector<unsigned char> expectedMessage{
         0x00, 0x00 // Data0, Data1
      };
      checkExpectedMessage(txMessage, expectedMessage);
   }

   BOOST_AUTO_TEST_CASE(NominalBigMessage)
   {
      std::vector<unsigned char> txMessage{
         kSTX, kSTX, 0x00, 0x12, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xAA, 0xCC, 0xB9, kETX // STX, STX, Datas, checksum, ETX
      };
      std::vector<unsigned char> expectedMessage{
         0x00, 0x12, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xAA, 0xCC // Datas
      };
      checkExpectedMessage(txMessage, expectedMessage);
   }

   BOOST_AUTO_TEST_CASE(BadChecksum)
   {
      std::vector<unsigned char> txMessage{
         kSTX, kSTX, 0x00, 0x00, 0xaa, kETX // STX, STX, Data0, Data1, bad checksum, ETX
      };
      checkFilteredMessage(txMessage);
   }

   BOOST_AUTO_TEST_CASE(Uncomplete)
   {
      std::vector<unsigned char> txMessage{
         kSTX, kSTX, 0x00, 0x00, 0x00, 0x00 // STX, STX, Data0, Data1... No ETX
      };
      checkFilteredMessage(txMessage);
   }

   BOOST_AUTO_TEST_CASE(Only2STX)
   {
      std::vector<unsigned char> txMessage{
         kSTX, kSTX,
      };
      checkFilteredMessage(txMessage);
   }

   BOOST_AUTO_TEST_CASE(bytesBeforeSTX)
   {
      std::vector<unsigned char> txMessage{
         0x01, 0x23, 0x56, kSTX, kSTX, 0x45, 0x67, 0x54, kETX // STX, STX, Data0, Data1, checksum, ETX
      };
      std::vector<unsigned char> expectedMessage{
         0x45, 0x67 // Data0, Data1
      };
      checkExpectedMessage(txMessage, expectedMessage);
   }

   BOOST_AUTO_TEST_CASE(bytesBeforeUncompleteMessageBeforeMessage)
   {
      std::vector<unsigned char> txMessage{
         0x01, 0x23, 0x56, // Invalid bytes
         kSTX, kSTX, 0x45, 0x67, 0x54, // Uncomplete message
         kSTX, kSTX, 0x00, 0x12, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xAA, 0xCC, 0xB9, kETX // True message
      };
      std::vector<unsigned char> expectedMessage{
         0x00, 0x12, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xAA, 0xCC // True message
      };
      checkExpectedMessage(txMessage, expectedMessage);
   }

   BOOST_AUTO_TEST_CASE(MultiplesMessages)
   {
      std::vector<unsigned char> txMessage{
         0x01, 0x23, 0x23, // Invalid bytes
         kSTX, kSTX, 0x45, 0x67, 0x54, kETX, // True message
         kSTX, kSTX, 0x00, 0x12, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xAA, 0xCC, 0xB9, kETX, // True message
         kSTX, kSTX, 0x00, 0x12, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x2E, kETX, // Bad checksum
         kSTX, kSTX, 0x00, 0x12, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x2F, kETX // True message
      };
      std::vector<unsigned char> expectedMessage1{
         0x45, 0x67
      };
      std::vector<unsigned char> expectedMessage2{
         0x00, 0x12, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xAA, 0xCC
      };
      std::vector<unsigned char> expectedMessage3{
         0x00, 0x12, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF
      };
      checkExpectedMessages(txMessage,
                            std::vector<std::vector<unsigned char>>{
                               expectedMessage1,
                               expectedMessage2,
                               expectedMessage3});
   }

   BOOST_AUTO_TEST_CASE(bytesAfterMessage)
   {
      std::vector<unsigned char> txMessage{
         kSTX, kSTX, 0x45, 0x67, 0x54, kETX, 0x12, 0x45 // STX, STX, Data0, Data1, checksum, ETX, invalid bytes
      };
      std::vector<unsigned char> expectedMessage{
         0x45, 0x67 // Data0, Data1
      };
      checkExpectedMessage(txMessage, expectedMessage);
   }
   
   BOOST_AUTO_TEST_CASE(CuttedMessage)
   {
      std::vector<unsigned char> txMessagePart1{
         kSTX, kSTX, 0x00
      };
      std::vector<unsigned char> txMessagePart2{
         0x12, 0x23, 0x45, 0x67
      };
      std::vector<unsigned char> txMessagePart3{
         0x89, 0xAB, 0xCD, 0xEF, 0xAA, 0xCC, 0xB9, kETX
      };
      std::vector<unsigned char> expectedMessage{
         0x00, 0x12, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xAA, 0xCC
      };

      auto timeProviderMock = useTimeMock();
      shared::event::CEventHandler evtHandler;
      CPicBootReceiveBufferHandler bufferHandler(evtHandler, shared::event::kUserFirstId, boost::posix_time::seconds(1), noLogger);

      bufferHandler.push(shared::communication::CByteBuffer(txMessagePart1));
      timeProviderMock->sleep(boost::posix_time::milliseconds(500));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent) ;

      bufferHandler.push(shared::communication::CByteBuffer(txMessagePart2));
      timeProviderMock->sleep(boost::posix_time::milliseconds(500));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent) ;
      
      bufferHandler.push(shared::communication::CByteBuffer(txMessagePart3));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId) ;

      auto rxMessage = evtHandler.getEventData<const std::vector<unsigned char>>();
      BOOST_CHECK_EQUAL_COLLECTIONS(
         rxMessage.begin(), rxMessage.end(),
         expectedMessage.begin(), expectedMessage.end()) ;
   }

   BOOST_AUTO_TEST_CASE(Timeout)
   {
      std::vector<unsigned char> txMessagePart1{
         kSTX, kSTX, 0x00
      };
      std::vector<unsigned char> txMessagePart2{
         0x12, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xAA, 0xCC, 0xB9, kETX
      };
      std::vector<unsigned char> txNextMessagePart{
         kSTX, kSTX, 0x00, 0x12, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xAA, 0xCC, 0xB9, kETX
      };
      std::vector<unsigned char> expectedMessage{
         0x00, 0x12, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xAA, 0xCC
      };

      auto timeProviderMock = useTimeMock();
      shared::event::CEventHandler evtHandler;
      CPicBootReceiveBufferHandler bufferHandler(evtHandler, shared::event::kUserFirstId, boost::posix_time::seconds(1), noLogger);

      bufferHandler.push(shared::communication::CByteBuffer(txMessagePart1));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent) ;
      timeProviderMock->sleep(boost::posix_time::milliseconds(1001));

      bufferHandler.push(shared::communication::CByteBuffer(txMessagePart2));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent) ;
      timeProviderMock->sleep(boost::posix_time::milliseconds(2000));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent) ;

      bufferHandler.push(shared::communication::CByteBuffer(txNextMessagePart));
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kUserFirstId) ;

      auto rxMessage = evtHandler.getEventData<const std::vector<unsigned char>>();
      BOOST_CHECK_EQUAL_COLLECTIONS(
         rxMessage.begin(), rxMessage.end(),
         expectedMessage.begin(), expectedMessage.end()) ;
   }   

   BOOST_AUTO_TEST_CASE(EscapedCharacters)
   {
      std::vector<unsigned char> txMessage{
         kSTX, kSTX, 0x00, 0x12, kDLE, kSTX, 0x23, 0x45, kDLE, kETX, 0x67, kDLE, kDLE, 0x89, 0xAB, 0xCD, 0xEF, 0xAA, 0xCC, 0x5B, kETX
      };
      std::vector<unsigned char> expectedMessage{
         0x00, 0x12, kSTX, 0x23, 0x45, kETX, 0x67, kDLE, 0x89, 0xAB, 0xCD, 0xEF, 0xAA, 0xCC
      };
      checkExpectedMessage(txMessage, expectedMessage);
   }

   BOOST_AUTO_TEST_CASE(EscapedCharactersAtLastPosition)
   {
      std::vector<unsigned char> txMessage{
         kSTX, kSTX, 0x01, 0x01, 0x00, 0x00, 0xFF, 0x47, kDLE
      };
      BOOST_CHECK_NO_THROW(checkFilteredMessage(txMessage));

      std::vector<unsigned char> txMessage2{
         kSTX, kSTX, 0x01, 0x01, 0x00, 0x00, 0xFF, 0x47, kDLE, kETX
      };
      BOOST_CHECK_NO_THROW(checkFilteredMessage(txMessage2));
   }
   
   BOOST_AUTO_TEST_SUITE_END()


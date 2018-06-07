#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/EnOcean/ReceiveBufferHandler.h"
#include "../../../../sources/plugins/EnOcean/MessageHandler.h"

#include "BufferLoggerMock.hpp"
#include "AsyncPortMock.hpp"
#include "ReceiveBufferHandlerMock.hpp"
#include "shared/currentTime/DefaultCurrentTimeMock.h"


class CFakeMessage : public message::CEsp3SendPacket
{
public:
   CFakeMessage()
      : CEsp3SendPacket(message::RADIO_ERP1)
   {
   }

   virtual ~CFakeMessage()
   {
   }
};

BOOST_AUTO_TEST_SUITE(TestMessageHandler)

   BOOST_AUTO_TEST_CASE(TestNominalReceive)
   {
      shared::event::CEventHandler evtHandler;
      enum
         {
            kDataReceived = shared::event::kUserFirstId
         };
      CMessageHandler messageHandler(boost::make_shared<CAsyncPortMock>(),
                                     evtHandler,
                                     kDataReceived,
                                     boost::make_shared<CBufferLoggerMock>());

      BOOST_CHECK(evtHandler.empty()) ;

      auto message = boost::make_shared<message::CEsp3ReceivedPacket>(std::vector<unsigned char>
         {0x55, 0x00, 0x00, 0x00, message::RADIO_ERP1, 0x07, 0x00});
      messageHandler.onReceived(message);

      // Check that received message was transmitted to event handler by the message handler
      BOOST_CHECK(evtHandler.waitForEvents(boost::date_time::min_date_time) == kDataReceived) ;
      BOOST_CHECK(evtHandler.getEventData<boost::shared_ptr<const message::CEsp3ReceivedPacket>>() == message) ;
   }


   BOOST_AUTO_TEST_CASE(TestNominalSend)
   {
      auto portMock = boost::make_shared<CAsyncPortMock>();
      shared::event::CEventHandler evtHandler;
      CMessageHandler messageHandler(portMock,
                                     evtHandler,
                                     shared::event::kUserFirstId,
                                     boost::make_shared<CBufferLoggerMock>());

      BOOST_CHECK(evtHandler.empty()) ;

      CFakeMessage message;
      messageHandler.send(message);

      // Check that sent message through message handler is really transmitted to port
      BOOST_CHECK_EQUAL(portMock->m_lastSendBuffer->size(), message.buffer()->size()) ;
      {
         auto vect1 = portMock->m_lastSendBuffer;
         auto vect2 = *message.buffer();
         auto idx1 = vect1->begin();
         auto idx2 = vect2.begin();
         for (;
            idx1 != vect1->end() && idx2 != vect2.end();
            ++idx1 , ++idx2)
         {
            BOOST_CHECK_EQUAL(*idx1, *idx2) ;
         }
      }
   }


   BOOST_AUTO_TEST_CASE(TestNominalSendAndReceive)
   {
      auto timeProviderMock = useTimeMock();
      auto portMock = boost::make_shared<CAsyncPortMock>();
      shared::event::CEventHandler mainEvtHandler;
      enum
         {
            kDataReceived = shared::event::kUserFirstId
         };
      auto messageHandler = boost::make_shared<CMessageHandler>(portMock,
                                                                mainEvtHandler,
                                                                kDataReceived,
                                                                boost::make_shared<CBufferLoggerMock>());

      BOOST_CHECK(mainEvtHandler.empty()) ;

      CFakeMessage request;
      auto answer = boost::make_shared<message::CEsp3ReceivedPacket>(std::vector<unsigned char>
         {0x55, 0x00, 0x00, 0x00, message::RADIO_ERP1, 0x07, 0x00});

      // Make portMock to automatically send answer when send is called
      portMock->setReceiveBufferHandler(boost::make_shared<ReceiveBufferHandlerMock>(messageHandler));
      portMock->setAutoReceiveMessage(boost::make_shared<shared::communication::CByteBuffer>(answer->buffer()));

      // Send the message and process answer
      auto sendSuccess = messageHandler->send(request,
                                              [](boost::shared_ptr<const message::CEsp3ReceivedPacket>)
                                              {
                                                 return true;
                                              },
                                              [&answer](boost::shared_ptr<const message::CEsp3ReceivedPacket> receivedAnswer)
                                              {
                                                 // Check that received message was transmitted as answer
                                                 BOOST_TEST_CHECK(receivedAnswer->buffer() == answer->buffer(), boost::test_tools::per_element()) ;
                                              });
      BOOST_CHECK(sendSuccess) ;

      // Check that sent message through message handler was really transmitted to port
      BOOST_CHECK_EQUAL(portMock->m_lastSendBuffer->size(), request.buffer()->size()) ;
      {
         auto vect1 = portMock->m_lastSendBuffer;
         auto vect2 = *request.buffer();
         auto idx1 = vect1->begin();
         auto idx2 = vect2.begin();
         for (;
            idx1 != vect1->end() && idx2 != vect2.end();
            ++idx1 , ++idx2)
         {
            BOOST_CHECK_EQUAL(*idx1, *idx2) ;
         }
      }

      // Check that received message was not transmitted to main event handler by the message handler
      timeProviderMock->sleep(boost::posix_time::seconds(100));
      BOOST_REQUIRE_EQUAL(mainEvtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);

      // Break circular shared_ptr reference to free memory
      portMock->setReceiveBufferHandler(boost::shared_ptr<shared::communication::IReceiveBufferHandler>());
   }


   BOOST_AUTO_TEST_CASE(TestSendAndTimeoutWhenReceive)
   {
      auto timeProviderMock = useTimeMock();
      auto portMock = boost::make_shared<CAsyncPortMock>();
      shared::event::CEventHandler mainEvtHandler;
      enum
         {
            kDataReceived = shared::event::kUserFirstId
         };
      auto messageHandler = boost::make_shared<CMessageHandler>(portMock,
                                                                mainEvtHandler,
                                                                kDataReceived,
                                                                boost::make_shared<CBufferLoggerMock>());

      BOOST_CHECK(mainEvtHandler.empty()) ;

      CFakeMessage request;
      const auto answer = boost::make_shared<message::CEsp3ReceivedPacket>(std::vector<unsigned char>
         {0x55, 0x00, 0x00, 0x00, message::RADIO_ERP1, 0x07, 0x00});

      // Send the message and process answer
      auto sendSuccess = messageHandler->send(request,
                                              [](boost::shared_ptr<const message::CEsp3ReceivedPacket>)
                                              {
                                                 // Should not receive any message
                                                 BOOST_CHECK(false) ;
                                                 return true;
                                              },
                                              [](boost::shared_ptr<const message::CEsp3ReceivedPacket>)
                                              {
                                                 // Should not receive any message
                                                 BOOST_CHECK(false) ;
                                              });
      BOOST_CHECK(!sendSuccess) ;

      // Check that received message was not transmitted to main event handler by the message handler
      timeProviderMock->sleep(boost::posix_time::seconds(100));
      BOOST_REQUIRE_EQUAL(mainEvtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);

      // Break circular shared_ptr reference to free memory
      portMock->setReceiveBufferHandler(boost::shared_ptr<shared::communication::IReceiveBufferHandler>());
   }

BOOST_AUTO_TEST_SUITE_END()


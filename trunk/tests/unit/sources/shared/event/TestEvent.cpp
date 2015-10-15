#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include <iostream>
#include <fstream>
#include <queue>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include "../../../../sources/shared/shared/event/EventHandler.hpp"

#include "../../mock/shared/currentTime/DefaultCurrentTimeMock.hpp"

BOOST_AUTO_TEST_SUITE(TestEvent)

// Event IDs
enum
{
   idEvent = shared::event::kUserFirstId,   // Always start from shared::event::kUserFirstId
};

class CEventData
{
public:
   CEventData(int intValue, const std::string& strValue)
      :m_intValue(intValue), m_strValue(strValue) { }
   CEventData()
      :m_intValue(0), m_strValue(std::string()) { }
   virtual ~CEventData() {}

   int intValue() { return m_intValue; }
   std::string strValue() { return m_strValue; }

private:
   int m_intValue;
   std::string m_strValue;
};


void postEventThreaded(shared::event::CEventHandler* receiver, int nbEvents)
{
   for (int counter = 0; counter < nbEvents; counter++)
      receiver->postEvent(idEvent);
}

void postEventWithDataThreaded(shared::event::CEventHandler* receiver, CEventData& data, int nbEvents)
{
   for (int counter = 0; counter < nbEvents; counter++)
      receiver->postEvent(idEvent, data);
}


//--------------------------------------------------------------
/// \brief	    Test Event sent from same thread
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(EventFromSameThread)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   shared::event::CEventHandler evtHandler;

   evtHandler.postEvent(idEvent);
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), idEvent);

   evtHandler.postEvent(idEvent);
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::seconds(1)), idEvent);

   evtHandler.postEvent(idEvent);
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::milliseconds(1)), idEvent);

   evtHandler.postEvent(idEvent);
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::pos_infin), idEvent);

   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::milliseconds(1)), shared::event::kTimeout);
}

//--------------------------------------------------------------
/// \brief	    Test Event sent from separate thread
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(EventFromSeparateThread)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   shared::event::CEventHandler evtHandler;

   {
      boost::thread postEventThreaded(postEventThreaded, &evtHandler, 1);
      postEventThreaded.join();
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), idEvent);
   }

   {
      boost::thread postEventThreaded(postEventThreaded, &evtHandler, 1);
      postEventThreaded.join();
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::seconds(1)), idEvent);
   }

   {
      boost::thread postEventThreaded(postEventThreaded, &evtHandler, 1);
      postEventThreaded.join();
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::milliseconds(1)), idEvent);
   }

   {
      boost::thread postEventThreaded(postEventThreaded, &evtHandler, 1);
      postEventThreaded.join();
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::pos_infin), idEvent);
   }

   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::milliseconds(1)), shared::event::kTimeout);
}

//--------------------------------------------------------------
/// \brief	    Test Events with 100 events from same thread
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(_100EventsFromSameThread)
{
   shared::event::CEventHandler evtHandler;
   
   for (int i = 0; i < 100; ++i)
      evtHandler.postEvent(idEvent);

   for (int i = 0; i < 100; ++i)
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::seconds(1)), idEvent);

   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::milliseconds(1)), shared::event::kTimeout);
}

//--------------------------------------------------------------
/// \brief	    Test Events with 100 events from separate thread
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(_100EventsFromSeparateThread)
{
   shared::event::CEventHandler evtHandler;

   boost::thread postEventThreaded(postEventThreaded, &evtHandler, 100);

   for (int i = 0; i < 100; ++i)
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::seconds(1)), idEvent);

   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::milliseconds(1)), shared::event::kTimeout);

   postEventThreaded.join();
}

//--------------------------------------------------------------
/// \brief	    Test Event with data sent from same thread
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(EventWithDataFromSameThread)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   shared::event::CEventHandler evtHandler;
   CEventData data(42, "Yadoms test");
   CEventData receivedData;

   evtHandler.postEvent(idEvent, data);
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), idEvent);
   BOOST_REQUIRE_NO_THROW(receivedData = evtHandler.getEventData<CEventData>());
   BOOST_CHECK_EQUAL(receivedData.intValue(), 42);
   BOOST_CHECK_EQUAL(receivedData.strValue(), std::string("Yadoms test"));
}

//--------------------------------------------------------------
/// \brief	    Test Event with data sent from separate thread
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(EventWithDataFromSeparateThread)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   shared::event::CEventHandler evtHandler;
   CEventData data(42, "Yadoms test");
   CEventData receivedData;

   boost::thread postEventWithDataThreaded(postEventWithDataThreaded, &evtHandler, data, 1);
   postEventWithDataThreaded.join();
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), idEvent);
   BOOST_REQUIRE_NO_THROW(receivedData = evtHandler.getEventData<CEventData>());
   BOOST_CHECK_EQUAL(receivedData.intValue(), 42);
   BOOST_CHECK_EQUAL(receivedData.strValue(), std::string("Yadoms test"));
}

//--------------------------------------------------------------
/// \brief	    Test Events sent from several separate threads
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(EventFromSeveralThreads)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   shared::event::CEventHandler evtHandler;

   // 100 threads sending each one event
   std::vector<boost::thread> threads;
   for (int i = 0; i < 100; ++i)
      threads.push_back(boost::thread(postEventThreaded, &evtHandler, 1));

   for (int i = 0; i < 100; ++i)
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::milliseconds(100)), idEvent);

   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::milliseconds(1)), shared::event::kTimeout);

   for (std::vector<boost::thread>::iterator itThread = threads.begin(); itThread != threads.end(); ++itThread)
      itThread->join();
}

//--------------------------------------------------------------
/// \brief	    Test Events with data sent from several separate threads
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(EventWithDataFromSeveralThreads)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   shared::event::CEventHandler evtHandler;
   std::vector<CEventData> data;
   std::set<int> expectedDataIntValue;
   for (int i = 0; i < 100; ++i)
   {
      data.push_back(CEventData(42 + i, "Yadoms test"));
      expectedDataIntValue.insert(42 + i);
   }

   // 100 threads sending each one event
   std::vector<boost::thread> threads;
   for (int i = 0; i < 100; ++i)
      threads.push_back(boost::thread(postEventWithDataThreaded, &evtHandler, data[i], 1));

   // Note that received event are not ncessary in sent order
   for (int i = 0; i < 100; ++i)
   {
      CEventData receivedData;
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::milliseconds(100)), idEvent);
      BOOST_REQUIRE_NO_THROW(receivedData = evtHandler.getEventData<CEventData>());
      BOOST_CHECK_EQUAL(receivedData.strValue(), std::string("Yadoms test"));
      // Remove the received data from the expected data list (should be empty at end)
      expectedDataIntValue.erase(receivedData.intValue());
   }

   BOOST_CHECK(expectedDataIntValue.empty());
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::milliseconds(1)), shared::event::kTimeout);

   for (std::vector<boost::thread>::iterator itThread = threads.begin(); itThread != threads.end(); ++itThread)
      itThread->join();
}

//--------------------------------------------------------------
/// \brief	    Test Event with bad data
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(EventWithBadData)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   shared::event::CEventHandler evtHandler;

   // Send string
   evtHandler.postEvent(idEvent, std::string("Yadoms test"));

   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), idEvent);

   BOOST_CHECK(!evtHandler.isEventType<CEventData>());
   BOOST_REQUIRE_THROW(evtHandler.getEventData<CEventData>(), shared::exception::CBadConversion);

   BOOST_CHECK(evtHandler.isEventType<std::string>());
   BOOST_REQUIRE_NO_THROW(evtHandler.getEventData<std::string>());


   // Send no data
   evtHandler.postEvent(idEvent);

   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), idEvent);

   BOOST_CHECK(!evtHandler.isEventType<CEventData>());
   BOOST_REQUIRE_THROW(evtHandler.getEventData<CEventData>(), shared::exception::CBadConversion);
}

//--------------------------------------------------------------
/// \brief	    Test empty event queue
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(EmptyEventHandler)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   shared::event::CEventHandler evtHandler;

   evtHandler.postEvent(idEvent);
   evtHandler.postEvent(idEvent, CEventData(42, "Yadoms test"));

   BOOST_CHECK(!evtHandler.empty());
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), idEvent);
   BOOST_CHECK(!evtHandler.empty());
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), idEvent);
   BOOST_CHECK(evtHandler.empty());
}

//--------------------------------------------------------------
/// \brief	    Test No wait event
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(NoWait)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   shared::event::CEventHandler evtHandler;

   evtHandler.postEvent(idEvent);

   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), idEvent);
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
}

//--------------------------------------------------------------
/// \brief	    Test event ID getter
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(EventIdGetter)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   shared::event::CEventHandler evtHandler;

   BOOST_REQUIRE_THROW(evtHandler.getEventId(), shared::exception::CNullReference);

   evtHandler.postEvent(idEvent);
   evtHandler.postEvent(idEvent + 1, CEventData(42, "Yadoms test"));

   BOOST_REQUIRE_THROW(evtHandler.getEventId(), shared::exception::CNullReference);

   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), idEvent);
   BOOST_CHECK_EQUAL(evtHandler.getEventId(), idEvent);
   BOOST_CHECK_EQUAL(evtHandler.getEventId(), idEvent); // Can be called several times
   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), idEvent + 1);
   BOOST_CHECK_EQUAL(evtHandler.getEventId(), idEvent + 1);
   BOOST_CHECK_EQUAL(evtHandler.getEventId(), idEvent + 1); // Can be called several times

   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
   BOOST_REQUIRE_THROW(evtHandler.getEventId(), shared::exception::CNullReference);
}

//--------------------------------------------------------------
/// \brief	    Test several events with data sent from several separate threads (heavy load test)
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(SeveralEventWithDataFromSeveralThreads)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   const unsigned int nbthreads = 100;
   const unsigned int nbMessagesPerThread = 1000;
   shared::event::CEventHandler evtHandler;
   std::vector<CEventData> data;
   std::map<int, int> expectedDataIntValueCounters;
   for (int i = 0; i < nbthreads; ++i)
   {
      data.push_back(CEventData(42 + i, "Yadoms test"));
      expectedDataIntValueCounters[42 + i] = nbMessagesPerThread;
   }

   // nbthreads threads sending each nbMessagesPerThread events
   std::vector<boost::thread> threads;
   for (int i = 0; i < nbthreads; ++i)
      threads.push_back(boost::thread(postEventWithDataThreaded, &evtHandler, data[i], nbMessagesPerThread));

   // Note that received event are not ncessary in sent order
   for (int i = 0; i < nbthreads * nbMessagesPerThread; ++i)
   {
      CEventData receivedData;
      BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::milliseconds(100)), idEvent);
      BOOST_REQUIRE_NO_THROW(receivedData = evtHandler.getEventData<CEventData>());
      BOOST_CHECK_EQUAL(receivedData.strValue(), std::string("Yadoms test"));
      // Remove the received data from the expected data list (should be empty at end)
      expectedDataIntValueCounters[receivedData.intValue()] --;
   }

   BOOST_CHECK_EQUAL(expectedDataIntValueCounters.size(), nbthreads);
   for (std::map<int, int>::iterator itCounter = expectedDataIntValueCounters.begin(); itCounter != expectedDataIntValueCounters.end(); ++itCounter)
      BOOST_CHECK_EQUAL(itCounter->second, 0);

   BOOST_CHECK_EQUAL(evtHandler.waitForEvents(boost::posix_time::milliseconds(1)), shared::event::kTimeout);

   for (std::vector<boost::thread>::iterator itThread = threads.begin(); itThread != threads.end(); ++itThread)
      itThread->join();
}

//--------------------------------------------------------------
/// \brief	    Shutdown test with several events sent simultaneously by several threads
/// \result         No Error
//--------------------------------------------------------------
void shutdownWhenSendingThreaded(boost::shared_ptr<shared::event::CEventHandler> eventHandler, int nbEvents)
{
   for (int iEvent = 0; iEvent < nbEvents; iEvent++)
      eventHandler->postEvent(idEvent);
}
BOOST_AUTO_TEST_CASE(ShutdownWhenSending)
{
   static shared::currentTime::Provider timeProvider(boost::make_shared<CDefaultCurrentTimeMock>());

   const unsigned int nbthreads = 100;
   const unsigned int nbMessagesPerThread = 1000;
   boost::shared_ptr<shared::event::CEventHandler> evtHandler(boost::make_shared<shared::event::CEventHandler>());

   // nbthreads threads sending each nbMessagesPerThread events
   std::vector<boost::thread> threads;
   for (int i = 0; i < nbthreads; ++i)
      threads.push_back(boost::thread(shutdownWhenSendingThreaded, evtHandler, nbMessagesPerThread));

   // Note that received event are not ncessary in sent order
   unsigned int nbReceivedEvents = 0;
   for (unsigned int i = 0; i < (nbthreads * nbMessagesPerThread) - 150; ++i)
   {
      BOOST_CHECK_EQUAL(evtHandler->waitForEvents(boost::posix_time::milliseconds(100)), idEvent);
      nbReceivedEvents++;
   }

   // Delete event handler here
   evtHandler.reset();

   for (std::vector<boost::thread>::iterator itThread = threads.begin(); itThread != threads.end(); ++itThread)
      itThread->join();
   
   BOOST_CHECK_EQUAL(nbReceivedEvents, (nbthreads * nbMessagesPerThread) - 150);
}

BOOST_AUTO_TEST_SUITE_END()
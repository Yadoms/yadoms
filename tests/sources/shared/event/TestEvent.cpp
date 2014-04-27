#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include <iostream>
#include <fstream>
#include <queue>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include "../../../../sources/shared/shared/event/EventHandler.hpp"

//#define DEBUG

BOOST_AUTO_TEST_SUITE(TestEvent)


// Event IDs
enum
{
   FirstMessage = shared::event::kUserFirstId,   // Always start from shared::event::kUserFirstId
   Message1,
   Message2,
   Message3,
   Message4,
   Message5
};

#define SIZE_DATA 10
class eventData
{
public:
   eventData()
   {
      for (int counter = 0; counter < SIZE_DATA; counter++)
         m_tab[counter] = counter;
   }

   ~eventData() {}

   bool isValid()
   {
      for (int counter = 0; counter < SIZE_DATA; counter++)
         if (m_tab[counter] != counter)
            return false;

      return true;
   }
private:
   int m_tab[10];
};



int ReceiverFirstMessageCounter;
int ReceiverMessage1Counter;
int ReceiverMessage2Counter;
int ReceiverMessage3Counter;
int ReceiverMessage4Counter;
int ReceiverMessage5Counter;
int ReceiverTimeoutCounter;
int ReceiverDefaultCounter;

static bool ReceiverMessageOrderOk;
static bool ReceiverMessageCorrect;

//static bool firstExecution;

void ThreadReceiver(shared::event::CEventHandler* receiver,int nbMessages)
{
   while ((ReceiverFirstMessageCounter + ReceiverMessage1Counter + ReceiverTimeoutCounter + ReceiverDefaultCounter ) != nbMessages)
   {  
      switch(receiver->waitForEvents(boost::posix_time::milliseconds(5000)))
      {
      case FirstMessage:
         {
            ReceiverFirstMessageCounter++;          
			break;
         }
      case Message1:
         {
            ReceiverMessage1Counter++;          
			break;
         }
      case shared::event::kTimeout:
         {
            ReceiverTimeoutCounter++;         
			break;
         }
      case shared::event::kNoEvent:
         {         
			break;
         }

      default:
         {
            ReceiverDefaultCounter++;        
			break;
         }
      }
   }
}

void ThreadReceiverWithMessages(shared::event::CEventHandler* receiver,int nbMessages)
{
	int messagePrecedent = 255;
	eventData DataReceived;
	bool exit = false;
	bool firstExecution = true;

	//TODO : Vérifier l'ordre des évènements et rajouter plusieurs messages

   while (!exit)
   {
      switch(receiver->waitForEvents(boost::posix_time::milliseconds(5000)))
      {
      case FirstMessage:
         {
            BOOST_REQUIRE_NO_THROW(DataReceived = receiver->getEventData<eventData>());
            ReceiverFirstMessageCounter++;
			messagePrecedent = 0;         
			break;
         }
      case Message1:
         {
            BOOST_REQUIRE_NO_THROW(DataReceived = receiver->getEventData<eventData>());
            ReceiverMessage1Counter++;
			if (messagePrecedent != 5 && !firstExecution)
				ReceiverMessageOrderOk = false;

			messagePrecedent = 1;         
			break;
         }
      case Message2:
         {
            BOOST_REQUIRE_NO_THROW(DataReceived = receiver->getEventData<eventData>());
            ReceiverMessage2Counter++;

			if (messagePrecedent != 1 && !firstExecution)
				ReceiverMessageOrderOk = false;

			messagePrecedent = 2;          
			break;
         }
      case Message3:
         {
            BOOST_REQUIRE_NO_THROW(DataReceived = receiver->getEventData<eventData>());
            ReceiverMessage3Counter++;

			if (messagePrecedent != 2 && !firstExecution)
				ReceiverMessageOrderOk = false;

			messagePrecedent = 3;          
			break;
         }
      case Message4:
         {
            BOOST_REQUIRE_NO_THROW(DataReceived = receiver->getEventData<eventData>());
            ReceiverMessage4Counter++;

			if (messagePrecedent != 3 && !firstExecution)
				ReceiverMessageOrderOk = false;

			messagePrecedent = 4;           
			break;
         }
      case Message5:
         {
            BOOST_REQUIRE_NO_THROW(DataReceived = receiver->getEventData<eventData>());
            ReceiverMessage5Counter++;

			if (messagePrecedent != 4 && !firstExecution)
				ReceiverMessageOrderOk = false;

			messagePrecedent = 5;          
			break;
         }
      case shared::event::kTimeout:
         {
            ReceiverTimeoutCounter++;
			messagePrecedent = 250;           
			break;
         }
      case shared::event::kNoEvent:
         {
			messagePrecedent = 251;     
			break;
         }
      default:
         {
            ReceiverDefaultCounter++;
			messagePrecedent = 252;   
			break;
         }
      }

	   //check the received Data
	if (messagePrecedent >0 && messagePrecedent <6)
		BOOST_CHECK_EQUAL( DataReceived.isValid(), true );

	  if  ((   ReceiverFirstMessageCounter + 
			   ReceiverMessage1Counter + 
			   ReceiverMessage2Counter + 
			   ReceiverMessage3Counter + 
			   ReceiverMessage4Counter + 
			   ReceiverMessage5Counter + 
			   ReceiverTimeoutCounter + 
			   ReceiverDefaultCounter ) >= nbMessages)
		exit = true;

	  firstExecution = false;
   }
}

void ThreadSender(shared::event::CEventHandler* receiver,int nbMessages)
{
	for (int counter = 0 ; counter < nbMessages; counter++)
	  receiver->sendEvent(Message1);
}

void ThreadSenderWithMessages(shared::event::CEventHandler* receiver,int nbMessages)
{
	eventData DataSent;

	for (int counter = 0 ; counter < nbMessages; counter++)
	  receiver->sendEvent<eventData>(Message1,DataSent);
}

void ThreadSenderWith5Messages(shared::event::CEventHandler* receiver,int nbMessages)
{
	eventData DataSent;

	for (int counter = 0 ; counter < nbMessages; counter++)
	{
	  receiver->sendEvent<eventData>(Message1,DataSent);
	  receiver->sendEvent<eventData>(Message2,DataSent);
	  receiver->sendEvent<eventData>(Message3,DataSent);
	  receiver->sendEvent<eventData>(Message4,DataSent);
	  receiver->sendEvent<eventData>(Message5,DataSent);
	}
}

void ThreadSenderWithMessagesString(shared::event::CEventHandler* receiver, int nbMessages)
{
	std::string DataSent = "Hello World";

	for (int counter = 0 ; counter < nbMessages; counter++)
	  receiver->sendEvent<std::string>(Message1,DataSent);
}

void InitReceiverCounter()
{
	ReceiverFirstMessageCounter = 0;
	ReceiverMessage1Counter     = 0;
	ReceiverMessage2Counter     = 0;
	ReceiverMessage3Counter     = 0;
	ReceiverMessage4Counter     = 0;
	ReceiverMessage5Counter     = 0;
	ReceiverTimeoutCounter      = 0;
	ReceiverDefaultCounter      = 0;
}

//--------------------------------------------------------------
/// \brief	    Test Events with 1 event sent - 1 Sender / 1 Receiver
/// \result         No Error
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Event_1_Frame)
{
	int nbOfFrames = 1;
	shared::event::CEventHandler evtHandler;

	InitReceiverCounter();

	boost::thread receiverThread(ThreadReceiver ,&evtHandler, nbOfFrames);
	boost::thread senderThread(ThreadSender     ,&evtHandler, nbOfFrames);

	senderThread.join();
	receiverThread.join();

	BOOST_CHECK_EQUAL(ReceiverFirstMessageCounter, 0);
	BOOST_CHECK_EQUAL(ReceiverMessage1Counter    , nbOfFrames);
	BOOST_CHECK_EQUAL(ReceiverTimeoutCounter     , 0);
	BOOST_CHECK_EQUAL(ReceiverDefaultCounter     , 0);
}

//--------------------------------------------------------------
/// \brief	    Test Events with 100 events sent simultaneously - 1 Sender / 1 Receiver
/// \result         No Error
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Event_100_Messages)
{
	int nbOfFrames = 100;
	shared::event::CEventHandler evtHandler;

	InitReceiverCounter();

	boost::thread receiverThread(ThreadReceiver , &evtHandler, nbOfFrames);
	boost::thread senderThread(ThreadSender     , &evtHandler, nbOfFrames);

	senderThread.join();
	receiverThread.join();

	BOOST_CHECK_EQUAL(ReceiverFirstMessageCounter, 0);
	BOOST_CHECK_EQUAL(ReceiverMessage1Counter    , nbOfFrames);
	BOOST_CHECK_EQUAL(ReceiverTimeoutCounter     , 0);
	BOOST_CHECK_EQUAL(ReceiverDefaultCounter     , 0);
}

//--------------------------------------------------------------
/// \brief	    Test Events with 1 event sent with Data - 1 Sender / 1 Receiver
/// \result         No Error
//--------------------------------------------------------------

// TODO : DEBUT_CODE_SG
void Event_1_Frame_with_Data_SG_senderThread(shared::event::CEventHandler* receiver)
{
	eventData DataSent;

	receiver->sendEvent<eventData>(FirstMessage, DataSent);
}

BOOST_AUTO_TEST_CASE(Event_1_Frame_with_Data_SG)//TODO : n'en conserver qu'un
{
   shared::event::CEventHandler evtHandler;

   // Data are sent by another thread
   boost::thread sender(Event_1_Frame_with_Data_SG_senderThread, &evtHandler);

   BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::posix_time::milliseconds(5000)), FirstMessage);
   eventData receivedData;
   BOOST_REQUIRE_NO_THROW(receivedData = evtHandler.getEventData<eventData>());
   BOOST_CHECK_EQUAL(receivedData.isValid(), true);

   sender.join();
}
// TODO : FIN_CODE_SG

BOOST_AUTO_TEST_CASE(Event_1_Frame_with_Data)
{
	int nbOfFrames        = 1;
	shared::event::CEventHandler evtHandler;

	InitReceiverCounter();

	boost::thread receiverThread(ThreadReceiverWithMessages ,&evtHandler, nbOfFrames);
	boost::thread senderThread(ThreadSenderWithMessages     ,&evtHandler, nbOfFrames);

	senderThread.join();
	receiverThread.join();

	BOOST_CHECK_EQUAL(ReceiverFirstMessageCounter, 0);
	BOOST_CHECK_EQUAL(ReceiverMessage1Counter    , nbOfFrames);
	BOOST_CHECK_EQUAL(ReceiverTimeoutCounter     , 0);
	BOOST_CHECK_EQUAL(ReceiverDefaultCounter     , 0);
}

//--------------------------------------------------------------
/// \brief	    Test Events with 100 events sent with Data - 1 Sender / 1 Receiver
/// \result         No Error
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Event_100_Frame_with_Data)
{
	int nbOfFrames        = 100;
	shared::event::CEventHandler evtHandler;

	InitReceiverCounter();

	boost::thread receiverThread(ThreadReceiverWithMessages ,&evtHandler, nbOfFrames);
	boost::thread senderThread(ThreadSenderWithMessages     ,&evtHandler, nbOfFrames);

	senderThread.join();
	receiverThread.join();

	BOOST_CHECK_EQUAL(ReceiverFirstMessageCounter, 0);
	BOOST_CHECK_EQUAL(ReceiverMessage1Counter    , nbOfFrames);
	BOOST_CHECK_EQUAL(ReceiverTimeoutCounter     , 0);
	BOOST_CHECK_EQUAL(ReceiverDefaultCounter     , 0);
}

//--------------------------------------------------------------
/// \brief	    Test Events with 1 event not sent - 1 Sender / 1 Receiver
/// \result         1 Message in TimeOut should be detected
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Event_TimeOut)
{
	int nbOfFramesSend     = 0;
	int nbOfFramesReceived = 1;
	shared::event::CEventHandler evtHandler;

	InitReceiverCounter();

	boost::thread receiverThread(ThreadReceiver ,&evtHandler, nbOfFramesReceived);
	boost::thread senderThread  (ThreadSender   ,&evtHandler, nbOfFramesSend    );

	senderThread.join();
	receiverThread.join();

	BOOST_CHECK_EQUAL(ReceiverFirstMessageCounter, 0);
	BOOST_CHECK_EQUAL(ReceiverMessage1Counter    , 0);
	BOOST_CHECK_EQUAL(ReceiverTimeoutCounter     , 1);
	BOOST_CHECK_EQUAL(ReceiverDefaultCounter     , 0);
}

//--------------------------------------------------------------
/// \brief	    Test Events with 50 events sent simultaneously by two senders - 2 Senders / 1 Receiver
/// \result         No Error
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Event_2_Senders_50_Messages_1_Receiver)
{
	int nbOfFramesSend     = 50;
	int nbOfFramesReceived = 100;
	shared::event::CEventHandler evtHandler;

	InitReceiverCounter();

	boost::thread receiverThread (ThreadReceiver ,&evtHandler, nbOfFramesReceived);
	boost::thread senderThread1  (ThreadSender   ,&evtHandler, nbOfFramesSend    );
	boost::thread senderThread2  (ThreadSender   ,&evtHandler, nbOfFramesSend    );

	senderThread1.join();
	senderThread2.join();
	receiverThread.join();

	BOOST_CHECK_EQUAL(ReceiverFirstMessageCounter, 0);
	BOOST_CHECK_EQUAL(ReceiverMessage1Counter    , nbOfFramesReceived);
	BOOST_CHECK_EQUAL(ReceiverTimeoutCounter     , 0);
	BOOST_CHECK_EQUAL(ReceiverDefaultCounter     , 0);
}

//--------------------------------------------------------------
/// \brief	    Test Events with 1 event sent simultaneously by 1 hundred senders - 100 Senders / 1 Receiver
/// \result         No Error
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Event_100_Senders_1_Message_1_Receiver)
{
	int nbOfFramesSend     = 1;
	int nbOfFramesReceived = 100;
	shared::event::CEventHandler evtHandler;

	InitReceiverCounter();

	boost::thread receiverThread (ThreadReceiver ,&evtHandler, nbOfFramesReceived);

	for (char counter = 0; counter < nbOfFramesReceived; counter++)
	{
		boost::thread senderThread  (ThreadSender,&evtHandler, nbOfFramesSend    );

		senderThread.join();
	}

	receiverThread.join();

	BOOST_CHECK_EQUAL(ReceiverFirstMessageCounter, 0);
	BOOST_CHECK_EQUAL(ReceiverMessage1Counter    , nbOfFramesReceived);
	BOOST_CHECK_EQUAL(ReceiverTimeoutCounter     , 0);
	BOOST_CHECK_EQUAL(ReceiverDefaultCounter     , 0);
}

//--------------------------------------------------------------
/// \brief	    Test Events with 1 event sent with Data - 1 Sender / 1 Receiver BadConversion
/// \result         Raise an Exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Event_1_Frame_with_Data_Exception)
{
	shared::event::CEventHandler evtHandler;

	evtHandler.sendEvent<std::string>(Message1,"Hello World");
	BOOST_REQUIRE_THROW (evtHandler.getEventData<eventData>(),shared::exception::CBadConversion);
}

//--------------------------------------------------------------
/// \brief	    Test Events with 500 events (5 differents messages) sent with Data - 1 Sender / 1 Receiver
/// \result         Messages arrives with the same order
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Event_100x5_Frames_Different_Messages_with_Data)
{
	int nbOfFramesSent      = 100;
	int nbOfFramesReceived  = 5 * nbOfFramesSent;
	shared::event::CEventHandler evtHandler;
//	ReceiverMessageCorrect = true; TODO : A supprimer
	ReceiverMessageOrderOk = true;

	InitReceiverCounter();

	boost::thread receiverThread(ThreadReceiverWithMessages  ,&evtHandler, nbOfFramesReceived);
	boost::thread senderThread  (ThreadSenderWith5Messages   ,&evtHandler, nbOfFramesSent);

	senderThread.join();
	receiverThread.join();
	
	BOOST_CHECK_EQUAL(ReceiverFirstMessageCounter, 0);
	BOOST_CHECK_EQUAL(ReceiverMessage1Counter    , nbOfFramesSent);
	BOOST_CHECK_EQUAL(ReceiverMessage2Counter    , nbOfFramesSent);
	BOOST_CHECK_EQUAL(ReceiverMessage3Counter    , nbOfFramesSent);
	BOOST_CHECK_EQUAL(ReceiverMessage4Counter    , nbOfFramesSent);
	BOOST_CHECK_EQUAL(ReceiverMessage5Counter    , nbOfFramesSent);
	BOOST_CHECK_EQUAL(ReceiverTimeoutCounter     , 0);
	BOOST_CHECK_EQUAL(ReceiverDefaultCounter     , 0);
//	BOOST_CHECK_EQUAL(ReceiverMessageCorrect     , true); // TODO A supprimer
	BOOST_CHECK_EQUAL(ReceiverMessageOrderOk     , true);
}

BOOST_AUTO_TEST_SUITE_END()
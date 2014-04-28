#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include <iostream>
#include <fstream>
#include <queue>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include "../../../../sources/shared/shared/event/EventHandler.hpp"

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
   int m_tab[SIZE_DATA];
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
static bool ReceiverMessageCorrect;//TODO à supprimer ?

class messageList
{
public:
	messageList(): m_position_Add(0), m_position_Valid(0) 
	{   // Initialisation of the tab
		for (int counter = 0; counter < 1000; counter++)
			m_Data[counter]=0;
	}
	~messageList(){}

	void addToList(int p_value)
	{
		m_Data[m_position_Add] = p_value;
		m_position_Add++;
	}

	bool isListIdentical(messageList& list)
	{
		for (int counter = 0; counter < 1000; counter++)
		{
			if (m_Data[m_position_Valid] != list.m_Data[m_position_Valid])
				return false;
		}
		return true;
	}
protected:
  int m_Data[1000];
  int m_position_Add;
  int m_position_Valid;
};

messageList listMessages;

void ThreadReceiver(shared::event::CEventHandler* receiver,int nbMessages)
{
	messageList ReceivedMessagesList;

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
			ReceivedMessagesList.addToList(1);
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
   if (ReceiverTimeoutCounter == 0)
     BOOST_CHECK_EQUAL(ReceivedMessagesList.isListIdentical(listMessages), true);
}

void ThreadReceiverWithMessages(shared::event::CEventHandler* receiver,int nbMessages)
{
	eventData DataReceived;
	bool exit = false;
	messageList ReceivedMessagesList;

   while (!exit)
   {
      switch(receiver->waitForEvents(boost::posix_time::milliseconds(5000)))
      {
      case FirstMessage:
         {
            BOOST_REQUIRE_NO_THROW(DataReceived = receiver->getEventData<eventData>());
            ReceiverFirstMessageCounter++;
			break;
         }
      case Message1:
         {
            BOOST_REQUIRE_NO_THROW(DataReceived = receiver->getEventData<eventData>());
            ReceiverMessage1Counter++;

			ReceivedMessagesList.addToList(1);

			BOOST_CHECK_EQUAL( DataReceived.isValid(), true );
			break;
         }
      case Message2:
         {
            BOOST_REQUIRE_NO_THROW(DataReceived = receiver->getEventData<eventData>());
            ReceiverMessage2Counter++;

			ReceivedMessagesList.addToList(2);

			BOOST_CHECK_EQUAL( DataReceived.isValid(), true );
			break;
         }
      case Message3:
         {
            BOOST_REQUIRE_NO_THROW(DataReceived = receiver->getEventData<eventData>());
            ReceiverMessage3Counter++;

			ReceivedMessagesList.addToList(3);

			BOOST_CHECK_EQUAL( DataReceived.isValid(), true );
			break;
         }
      case Message4:
         {
            BOOST_REQUIRE_NO_THROW(DataReceived = receiver->getEventData<eventData>());
            ReceiverMessage4Counter++;

			ReceivedMessagesList.addToList(4);

			BOOST_CHECK_EQUAL( DataReceived.isValid(), true );
			break;
         }
      case Message5:
         {
            BOOST_REQUIRE_NO_THROW(DataReceived = receiver->getEventData<eventData>());
            ReceiverMessage5Counter++;

			ReceivedMessagesList.addToList(5);

			BOOST_CHECK_EQUAL( DataReceived.isValid(), true );
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

	  if  ((   ReceiverFirstMessageCounter + 
			   ReceiverMessage1Counter + 
			   ReceiverMessage2Counter + 
			   ReceiverMessage3Counter + 
			   ReceiverMessage4Counter + 
			   ReceiverMessage5Counter + 
			   ReceiverTimeoutCounter + 
			   ReceiverDefaultCounter ) >= nbMessages)
		exit = true;
   }
   if (ReceiverTimeoutCounter == 0)
     BOOST_CHECK_EQUAL(ReceivedMessagesList.isListIdentical(listMessages), true);
}

void ThreadSender(shared::event::CEventHandler* receiver,int nbMessages)
{
	for (int counter = 0 ; counter < nbMessages; counter++)
	{
	  receiver->sendEvent(Message1);
	  listMessages.addToList(1);
	}
}

void ThreadSenderWithMessages(shared::event::CEventHandler* receiver,int nbMessages)
{
	eventData DataSent;

	for (int counter = 0 ; counter < nbMessages; counter++)
	{
	  receiver->sendEvent<eventData>(Message1,DataSent);
	  listMessages.addToList(1);
	}
}

void ThreadSenderWith5Messages(shared::event::CEventHandler* receiver,int nbMessages)
{
	eventData DataSent;
	static int messageListPosition=0;

	for (int counter = 0 ; counter < nbMessages; counter++)
	{
	  receiver->sendEvent<eventData>(Message1,DataSent);
	  listMessages.addToList(1);
	  receiver->sendEvent<eventData>(Message2,DataSent);
	  listMessages.addToList(2);
	  receiver->sendEvent<eventData>(Message3,DataSent);
	  listMessages.addToList(3);
	  receiver->sendEvent<eventData>(Message4,DataSent);
	  listMessages.addToList(4);
	  receiver->sendEvent<eventData>(Message5,DataSent);
	  listMessages.addToList(5);
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

	boost::thread senderThread(ThreadSender     ,&evtHandler, nbOfFrames);

	senderThread.join();

	ThreadReceiver(&evtHandler, nbOfFrames);

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

	boost::thread senderThread(ThreadSender     , &evtHandler, nbOfFrames);

	senderThread.join();
	ThreadReceiver ( &evtHandler, nbOfFrames );

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

	boost::thread senderThread(ThreadSenderWithMessages     ,&evtHandler, nbOfFrames);

	senderThread.join();
	ThreadReceiverWithMessages(&evtHandler, nbOfFrames);

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

	boost::thread senderThread(ThreadSenderWithMessages     ,&evtHandler, nbOfFrames);

	senderThread.join();
	ThreadReceiverWithMessages(&evtHandler, nbOfFrames);

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

	boost::thread senderThread  (ThreadSender   ,&evtHandler, nbOfFramesSend    );

	senderThread.join();

	ThreadReceiver(&evtHandler, nbOfFramesReceived);

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

	boost::thread senderThread1  (ThreadSender   ,&evtHandler, nbOfFramesSend    );
	boost::thread senderThread2  (ThreadSender   ,&evtHandler, nbOfFramesSend    );

	senderThread1.join();
	senderThread2.join();
	ThreadReceiver(&evtHandler, nbOfFramesReceived);

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

	for (char counter = 0; counter < nbOfFramesReceived; counter++)
	{
		boost::thread senderThread  (ThreadSender,&evtHandler, nbOfFramesSend    );
		senderThread.join();
	}

	ThreadReceiver(&evtHandler, nbOfFramesReceived);

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
	BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), Message1);  // No wait
	BOOST_REQUIRE_THROW (evtHandler.getEventData<eventData>(),shared::exception::CBadConversion);
}

//--------------------------------------------------------------
/// \brief	    Test Events with 500 events (5 differents messages) sent with Data - 1 Sender / 1 Receiver
/// \result         Messages arrive with the same order
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Event_100x5_Frames_Different_Messages_with_Data)
{
	int nbOfFramesSent      = 100;
	int nbOfFramesReceived  = 5 * nbOfFramesSent;
	shared::event::CEventHandler evtHandler;
	ReceiverMessageOrderOk = true;

	InitReceiverCounter();

	boost::thread senderThread  (ThreadSenderWith5Messages   ,&evtHandler, nbOfFramesSent);

	senderThread.join();
	ThreadReceiverWithMessages(&evtHandler, nbOfFramesReceived);
	
	BOOST_CHECK_EQUAL(ReceiverFirstMessageCounter, 0);
	BOOST_CHECK_EQUAL(ReceiverMessage1Counter    , nbOfFramesSent);
	BOOST_CHECK_EQUAL(ReceiverMessage2Counter    , nbOfFramesSent);
	BOOST_CHECK_EQUAL(ReceiverMessage3Counter    , nbOfFramesSent);
	BOOST_CHECK_EQUAL(ReceiverMessage4Counter    , nbOfFramesSent);
	BOOST_CHECK_EQUAL(ReceiverMessage5Counter    , nbOfFramesSent);
	BOOST_CHECK_EQUAL(ReceiverTimeoutCounter     , 0);
	BOOST_CHECK_EQUAL(ReceiverDefaultCounter     , 0);
	BOOST_CHECK_EQUAL(ReceiverMessageOrderOk     , true);
}

//--------------------------------------------------------------
/// \brief	    Test one message with no WaitForEvent
/// \result         No Error ??
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Event_1_No_WaitForEvent)
{
	int nbOfFrames = 1;
	shared::event::CEventHandler evtHandler;

	boost::thread senderThread(ThreadSender     ,&evtHandler, nbOfFrames);

	senderThread.join();

	BOOST_REQUIRE_THROW (evtHandler.getEventData<eventData>(),shared::exception::CBadConversion); //TODO : A voir avec Seb -> Aujourd'hui, une erreur !
}

//--------------------------------------------------------------
/// \brief	    Test Events with 1 event sent with Data - 1 Sender / 1 Receiver BadConversion
/// \result         Raise an Exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Event_1_Frame_with_2_GetEventData)
{
	shared::event::CEventHandler evtHandler;
	eventData DataSent;
	eventData DataReceived;

	evtHandler.sendEvent<eventData>(Message1,DataSent);
	BOOST_REQUIRE_EQUAL (evtHandler.waitForEvents(boost::date_time::min_date_time), Message1);  // No wait
	BOOST_REQUIRE_NO_THROW ( DataReceived = evtHandler.getEventData<eventData>());
	BOOST_REQUIRE_EQUAL (DataReceived.isValid(), true);
	BOOST_REQUIRE_NO_THROW ( DataReceived = evtHandler.getEventData<eventData>());
	BOOST_REQUIRE_EQUAL (DataReceived.isValid(), true);
}

BOOST_AUTO_TEST_SUITE_END()
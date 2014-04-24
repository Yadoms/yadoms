#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include <iostream>
#include <fstream>
#include <queue>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include "../../../../sources/shared/shared/event/EventHandler.hpp"

#define DEBUG

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

struct structData
{
	int tab[10];
};

int ReceiverFirstMessageCounter;
int ReceiverMessage1Counter;
int ReceiverMessage2Counter;
int ReceiverMessage3Counter;
int ReceiverMessage4Counter;
int ReceiverMessage5Counter;
int ReceiverTimeoutCounter;
int ReceiverDefaultCounter;

bool ReceiverMessageOrderOk;
bool ReceiverMessageCorrect;

shared::event::CEventHandler EvtHandler;

void ThreadReceiver(long nbMessages)
{
   while ((ReceiverFirstMessageCounter + ReceiverMessage1Counter + ReceiverTimeoutCounter + ReceiverDefaultCounter ) != nbMessages)
   {
#ifdef DEBUG
      std::cout << "DEBUG Thread Receiver: WaitForEvents()" << std::endl;
#endif
      
      switch(EvtHandler.waitForEvents(boost::posix_time::milliseconds(5000)))
      {
      case FirstMessage:
         {
            EvtHandler.popEvent();
            ReceiverFirstMessageCounter++;
#ifdef DEBUG
            std::cout << "DEBUG Thread Receiver: First message received (#" << ReceiverFirstMessageCounter  << ")" << std::endl;
#endif            
			break;
         }
      case Message1:
         {
            EvtHandler.popEvent();
            ReceiverMessage1Counter++;
#ifdef DEBUG
            std::cout << "DEBUG Thread Receiver: Message 1 Received (#" << ReceiverMessage1Counter << ")" << std::endl;
#endif            
			break;
         }
      case shared::event::kTimeout:
         {
            ReceiverTimeoutCounter++;
#ifdef DEBUG            
			std::cout << "DEBUG Thread Receiver: TimeOut (#" << ReceiverTimeoutCounter << ")" << std::endl;
#endif            
			break;
         }
      case shared::event::kNoEvent:
         {
#ifdef DEBUG
            std::cout << "DEBUG Thread Receiver: NoEvent (#" << ReceiverTimeoutCounter << ")" << std::endl;
#endif            
			break;
         }

      default:
         {
            ReceiverDefaultCounter++;
#ifdef DEBUG            
			std::cout << "DEBUG Thread Receiver: Unknown message id (#" << ReceiverDefaultCounter << ")" << std::endl;
#endif            
			break;
         }
      }
   }
#ifdef DEBUG
   std::cout << "DEBUG Thread Receiver: finished" << std::endl;
#endif
}

void ThreadReceiverWithMessages(long nbMessages)
{
	int counter;
	int messagePrecedent = 255;
	structData DataReceived;
	int exit = false;

   while (!exit)
   {
	  if  ((   ReceiverFirstMessageCounter + 
			   ReceiverMessage1Counter + 
			   ReceiverMessage2Counter + 
			   ReceiverMessage3Counter + 
			   ReceiverMessage4Counter + 
			   ReceiverMessage5Counter + 
			   ReceiverTimeoutCounter + 
			   ReceiverDefaultCounter ) >= nbMessages)
		exit = true;

#ifdef DEBUG
      std::cout << "DEBUG Thread Receiver: WaitForEvents()" << std::endl;
#endif      
	  //Pretend to do something useful...
      switch(EvtHandler.waitForEvents(boost::posix_time::milliseconds(5000)))
      {
      case FirstMessage:
         {
            DataReceived = EvtHandler.popEvent<structData>();
            ReceiverFirstMessageCounter++;
			messagePrecedent = 0;
#ifdef DEBUG            
			std::cout << "DEBUG Thread Receiver: First message received (#" << ReceiverFirstMessageCounter  << ")" << std::endl;
#endif            
			break;
         }
      case Message1:
         {
            DataReceived = EvtHandler.popEvent<structData>();
            ReceiverMessage1Counter++;
			if (messagePrecedent != 5)
				ReceiverMessageOrderOk = ReceiverMessageOrderOk & false;

			messagePrecedent = 1;
#ifdef DEBUG            
			std::cout << "DEBUG Thread Receiver: Message 1 Received (#" << ReceiverMessage1Counter << ")" << std::endl;
#endif            
			break;
         }
      case Message2:
         {
            DataReceived = EvtHandler.popEvent<structData>();
            ReceiverMessage2Counter++;

			if (messagePrecedent != 1)
				ReceiverMessageOrderOk = ReceiverMessageOrderOk & false;

			messagePrecedent = 2;
#ifdef DEBUG            
			std::cout << "DEBUG Thread Receiver: Message 2 Received (#" << ReceiverMessage2Counter << ")" << std::endl;
#endif            
			break;
         }
      case Message3:
         {
            DataReceived = EvtHandler.popEvent<structData>();
            ReceiverMessage3Counter++;

			if (messagePrecedent != 2)
				ReceiverMessageOrderOk = ReceiverMessageOrderOk & false;

			messagePrecedent = 3;
#ifdef DEBUG            
			std::cout << "DEBUG Thread Receiver: Message 3 Received (#" << ReceiverMessage3Counter << ")" << std::endl;
#endif            
			break;
         }
      case Message4:
         {
            DataReceived = EvtHandler.popEvent<structData>();
            ReceiverMessage4Counter++;

			if (messagePrecedent != 3)
				ReceiverMessageOrderOk = ReceiverMessageOrderOk & false;

			messagePrecedent = 4;
#ifdef DEBUG            
			std::cout << "DEBUG Thread Receiver: Message 4 Received (#" << ReceiverMessage4Counter << ")" << std::endl;
#endif            
			break;
         }
      case Message5:
         {
            DataReceived = EvtHandler.popEvent<structData>();
            ReceiverMessage5Counter++;

			if (messagePrecedent != 4)
				ReceiverMessageOrderOk = ReceiverMessageOrderOk & false;

			messagePrecedent = 5;
#ifdef DEBUG            
			std::cout << "DEBUG Thread Receiver: Message 5 Received (#" << ReceiverMessage5Counter << ")" << std::endl;
#endif            
			break;
         }
      case shared::event::kTimeout:
         {
            ReceiverTimeoutCounter++;
			messagePrecedent = 250;
#ifdef DEBUG            
			std::cout << "DEBUG Thread Receiver: TimeOut (#" << ReceiverTimeoutCounter << ")" << std::endl;
#endif            
			break;
         }
      case shared::event::kNoEvent:
         {
			 messagePrecedent = 251;
#ifdef DEBUG
            std::cout << "DEBUG Thread Receiver: NoEvent (#" << ReceiverTimeoutCounter << ")" << std::endl;
#endif            
			break;
         }
      default:
         {
            ReceiverDefaultCounter++;
			messagePrecedent = 252;
#ifdef DEBUG            
			std::cout << "DEBUG Thread Receiver: Unknown message id (#" << ReceiverDefaultCounter << ")" << std::endl;
#endif            
			break;
         }
      }

	   //check the received Data

	   counter = 0;
	   while (counter < 10 && DataReceived.tab[counter]==counter)
		   counter++;

	   if (counter == 10)
	   {
		   ReceiverMessageCorrect = ReceiverMessageCorrect & true;
#ifdef DEBUG		   
		   std::cout << "DEBUG Data Received Correctly" << std::endl;
#endif	   
	   }
	   else
	   {
		   ReceiverMessageCorrect = ReceiverMessageCorrect & false;
#ifdef DEBUG		   
		   std::cout << "DEBUG Data Not Received Correctly" << std::endl;
#endif
	   }
   }

#ifdef DEBUG
   std::cout << "DEBUG Thread Receiver: finished" << std::endl;
#endif
}

void ThreadSender(long nbMessages)
{
	long i;

#ifdef DEBUG
	std::cout << "DEBUG Thread Sender: sendEvent (a)" << std::endl;
#endif
	for (i = 0 ; i < nbMessages; i++)
	{
	  EvtHandler.sendEvent(Message1);

#ifdef DEBUG
	  std::cout << "DEBUG Thread Sender: sendEvent(Message1) nb:" << i  << std::endl;
#endif
	}
#ifdef DEBUG
	std::cout << "DEBUG Thread Sender: finished" << std::endl;
#endif
}

void ThreadSenderWithMessages(long nbMessages)
{
	long counter;
	structData DataSent;

	for (counter = 0; counter < 10; counter++)
		DataSent.tab[counter] = counter;

#ifdef DEBUG
	std::cout << "DEBUG Thread Sender: sendEvent (a)" << std::endl;
#endif
	for (counter = 0 ; counter < nbMessages; counter++)
	{
	  EvtHandler.sendEvent<structData>(Message1,DataSent);

#ifdef DEBUG	  
	  std::cout << "DEBUG Thread Sender: sendEvent(Message1) nb:" << counter  << std::endl;
#endif
	}

#ifdef DEBUG	
	std::cout << "DEBUG Thread Sender: finished" << std::endl;
#endif
}

void ThreadSenderWithDifferentMessages(long nbMessages)
{
	long counter;
	structData DataSent;

	for (counter = 0; counter < 10; counter++)
		DataSent.tab[counter] = counter;

#ifdef DEBUG
	std::cout << "DEBUG Thread Sender: sendEvent (a)" << std::endl;
#endif
	for (counter = 0 ; counter < nbMessages; counter++)
	{
	  EvtHandler.sendEvent<structData>(Message1,DataSent);
	  EvtHandler.sendEvent<structData>(Message2,DataSent);
	  EvtHandler.sendEvent<structData>(Message3,DataSent);
	  EvtHandler.sendEvent<structData>(Message4,DataSent);
	  EvtHandler.sendEvent<structData>(Message5,DataSent);

#ifdef DEBUG	  
	  std::cout << "DEBUG Thread Sender: sendEvent(Message1) nb:" << counter  << std::endl;
#endif
	}

#ifdef DEBUG	
	std::cout << "DEBUG Thread Sender: finished" << std::endl;
#endif
}

void ThreadSenderWithMessagesString(long nbMessages)
{
	long counter;
	std::string DataSent = "Hello World";

#ifdef DEBUG
	std::cout << "DEBUG Thread Sender: sendEvent (a)" << std::endl;
#endif
	for (counter = 0 ; counter < nbMessages; counter++)
	{
	  EvtHandler.sendEvent<std::string>(Message1,DataSent);

#ifdef DEBUG
	  std::cout << "DEBUG Thread Sender: sendEvent(Message1) nb:" << counter  << std::endl;
#endif
	}

#ifdef DEBUG
	std::cout << "DEBUG Thread Sender: finished" << std::endl;
#endif
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

	InitReceiverCounter();

	boost::thread receiverThread(ThreadReceiver , nbOfFrames);
	boost::thread senderThread(ThreadSender     , nbOfFrames);

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

	InitReceiverCounter();

	boost::thread receiverThread(ThreadReceiver , nbOfFrames);
	boost::thread senderThread(ThreadSender     , nbOfFrames);

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

BOOST_AUTO_TEST_CASE(Event_1_Frame_with_Data)
{
	int nbOfFrames        = 1;
	ReceiverMessageCorrect = true;

	InitReceiverCounter();

	boost::thread receiverThread(ThreadReceiverWithMessages , nbOfFrames);
	boost::thread senderThread(ThreadSenderWithMessages     , nbOfFrames);

	senderThread.join();
	receiverThread.join();

	BOOST_CHECK_EQUAL(ReceiverFirstMessageCounter, 0);
	BOOST_CHECK_EQUAL(ReceiverMessage1Counter    , nbOfFrames);
	BOOST_CHECK_EQUAL(ReceiverTimeoutCounter     , 0);
	BOOST_CHECK_EQUAL(ReceiverDefaultCounter     , 0);
	BOOST_CHECK_EQUAL(ReceiverMessageCorrect     , true);
}

//--------------------------------------------------------------
/// \brief	    Test Events with 100 events sent with Data - 1 Sender / 1 Receiver
/// \result         No Error
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Event_100_Frame_with_Data)
{
	int nbOfFrames        = 100;
	ReceiverMessageCorrect = true;

	InitReceiverCounter();

	boost::thread receiverThread(ThreadReceiverWithMessages , nbOfFrames);
	boost::thread senderThread(ThreadSenderWithMessages     , nbOfFrames);

	senderThread.join();
	receiverThread.join();

	BOOST_CHECK_EQUAL(ReceiverFirstMessageCounter, 0);
	BOOST_CHECK_EQUAL(ReceiverMessage1Counter    , nbOfFrames);
	BOOST_CHECK_EQUAL(ReceiverTimeoutCounter     , 0);
	BOOST_CHECK_EQUAL(ReceiverDefaultCounter     , 0);
	BOOST_CHECK_EQUAL(ReceiverMessageCorrect     , true);
}

//--------------------------------------------------------------
/// \brief	    Test Events with 1 event not sent - 1 Sender / 1 Receiver
/// \result         1 Message in TimeOut should be detected
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Event_TimeOut)
{
	int nbOfFramesSend     = 0;
	int nbOfFramesReceived = 1;

	InitReceiverCounter();

	boost::thread receiverThread(ThreadReceiver , nbOfFramesReceived);
	boost::thread senderThread  (ThreadSender   , nbOfFramesSend    );

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

	InitReceiverCounter();

	boost::thread receiverThread (ThreadReceiver , nbOfFramesReceived);
	boost::thread senderThread1  (ThreadSender   , nbOfFramesSend    );
	boost::thread senderThread2  (ThreadSender   , nbOfFramesSend    );

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

	InitReceiverCounter();

	boost::thread receiverThread (ThreadReceiver , nbOfFramesReceived);

	for (char counter = 0; counter < nbOfFramesReceived; counter++)
	{
		boost::thread senderThread  (ThreadSender   , nbOfFramesSend    );

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
	EvtHandler.sendEvent<std::string>(Message1,"Hello World");

	BOOST_REQUIRE_THROW (EvtHandler.popEvent<structData>(),shared::exception::CBadConversion);
}

//--------------------------------------------------------------
/// \brief	    Test Events with 500 events (5 differents messages) sent with Data - 1 Sender / 1 Receiver
/// \result         Messages arrives with the same order
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Event_100x5_Frames_Different_Messages_with_Data)
{
	int nbOfFrames        = 2;
	ReceiverMessageCorrect = true;
	ReceiverMessageOrderOk = true;

	InitReceiverCounter();

	boost::thread receiverThread(ThreadReceiverWithMessages          , nbOfFrames);
	boost::thread senderThread  (ThreadSenderWithDifferentMessages   , nbOfFrames);

	senderThread.join();
	receiverThread.join();
	
	BOOST_CHECK_EQUAL(ReceiverFirstMessageCounter, 0);
	BOOST_CHECK_EQUAL(ReceiverMessage1Counter    , nbOfFrames);
	BOOST_CHECK_EQUAL(ReceiverMessage2Counter    , nbOfFrames);
	BOOST_CHECK_EQUAL(ReceiverMessage3Counter    , nbOfFrames);
	BOOST_CHECK_EQUAL(ReceiverMessage4Counter    , nbOfFrames);
	BOOST_CHECK_EQUAL(ReceiverMessage5Counter    , nbOfFrames);
	BOOST_CHECK_EQUAL(ReceiverTimeoutCounter     , 0);
	BOOST_CHECK_EQUAL(ReceiverDefaultCounter     , 0);
	BOOST_CHECK_EQUAL(ReceiverMessageCorrect     , true);
	BOOST_CHECK_EQUAL(ReceiverMessageOrderOk     , true);
}

BOOST_AUTO_TEST_SUITE_END()
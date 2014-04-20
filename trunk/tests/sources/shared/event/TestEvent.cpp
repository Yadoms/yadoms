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
   Message2
};

struct structData
{
	int maData[10];
};

long ReceiverFirstMessageCounter;
long ReceiverMessage1Counter;
long ReceiverTimeoutCounter;
long ReceiverDefaultCounter;

bool ReceiverMessageCorrect;

shared::event::CEventHandler EvtHandler;

void ThreadReceiver(long nbMessages)
{
   while ((ReceiverFirstMessageCounter + ReceiverMessage1Counter + ReceiverTimeoutCounter + ReceiverDefaultCounter ) != nbMessages)
   {

      //boost::posix_time::seconds workTime(1);

      std::cout << "Thread Receiver: WaitForEvents()" << std::endl;
      //Pretend to do something useful...
      switch(EvtHandler.waitForEvents(boost::posix_time::milliseconds(5000)))
      {
      case FirstMessage:
         {
            EvtHandler.popEvent();
            ReceiverFirstMessageCounter++;
            std::cout << "Thread Receiver: First message received (#" << ReceiverFirstMessageCounter  << ")" << std::endl;
            break;
         }
      case Message1:
         {
            EvtHandler.popEvent();
            ReceiverMessage1Counter++;
            std::cout << "Thread Receiver: Message 1 Received (#" << ReceiverMessage1Counter << ")" << std::endl;
            break;
         }
      case shared::event::kTimeout:
         {
            ReceiverTimeoutCounter++;
            std::cout << "Thread Receiver: TimeOut (#" << ReceiverTimeoutCounter << ")" << std::endl;
            break;
         }
      case shared::event::kNoEvent:
         {
            //ReceiverTimeoutCounter++;
            std::cout << "Thread Receiver: NoEvent (#" << ReceiverTimeoutCounter << ")" << std::endl;
            break;
         }
      default:
         {
            ReceiverDefaultCounter++;
            std::cout << "Thread Receiver: Unknown message id (#" << ReceiverDefaultCounter << ")" << std::endl;
            break;
         }
      }
   }
   std::cout << "Thread Receiver: finished" << std::endl;
}

void ThreadReceiverWithMessages(long nbMessages)
{
	int counter;
	structData DataReceived;

   while ((ReceiverFirstMessageCounter + ReceiverMessage1Counter + ReceiverTimeoutCounter + ReceiverDefaultCounter ) != nbMessages)
   {

      //boost::posix_time::seconds workTime(1);

      std::cout << "Thread Receiver: WaitForEvents()" << std::endl;
      //Pretend to do something useful...
      switch(EvtHandler.waitForEvents(boost::posix_time::milliseconds(5000)))
      {
      case FirstMessage:
         {
            DataReceived = EvtHandler.popEvent<structData>();
            ReceiverFirstMessageCounter++;
            std::cout << "Thread Receiver: First message received (#" << ReceiverFirstMessageCounter  << ")" << std::endl;
            break;
         }
      case Message1:
         {
            DataReceived = EvtHandler.popEvent<structData>();
            ReceiverMessage1Counter++;
            std::cout << "Thread Receiver: Message 1 Received (#" << ReceiverMessage1Counter << ")" << std::endl;
            break;
         }
      case shared::event::kTimeout:
         {
            ReceiverTimeoutCounter++;
            std::cout << "Thread Receiver: TimeOut (#" << ReceiverTimeoutCounter << ")" << std::endl;
            break;
         }
      case shared::event::kNoEvent:
         {
            //ReceiverTimeoutCounter++;
            std::cout << "Thread Receiver: NoEvent (#" << ReceiverTimeoutCounter << ")" << std::endl;
            break;
         }
      default:
         {
            ReceiverDefaultCounter++;
            std::cout << "Thread Receiver: Unknown message id (#" << ReceiverDefaultCounter << ")" << std::endl;
            break;
         }
      }

	   //check the received Data

	   counter = 0;
	   while (counter < 10 && DataReceived.maData[counter]==counter)
		   counter++;

	   if (counter == 10)
	   {
		   ReceiverMessageCorrect = ReceiverMessageCorrect & true;
		   std::cout << "Data Received Correctly" << std::endl;
	   }
	   else
	   {
		   ReceiverMessageCorrect = ReceiverMessageCorrect & false;
		   std::cout << "Data Not Received Correctly" << std::endl;
	   }
   }

   std::cout << "Thread Receiver: finished" << std::endl;
}

void ThreadSender(long nbMessages)
{
	long i;

	std::cout << "Thread Sender: sendEvent (a)" << std::endl;
	for (i = 0 ; i < nbMessages; i++)
	{
	  EvtHandler.sendEvent(Message1);

	  std::cout << "Thread Sender: sendEvent(Message1) nb:" << i  << std::endl;
	}

	//boost::this_thread::sleep(workTime);
	std::cout << "Thread Sender: finished" << std::endl;
}

void ThreadSenderWithMessages(long nbMessages)
{
	long counter;
	structData DataSent;

	for (counter = 0; counter < 10; counter++)
		DataSent.maData[counter] = counter;

	std::cout << "Thread Sender: sendEvent (a)" << std::endl;
	for (counter = 0 ; counter < nbMessages; counter++)
	{
	  EvtHandler.sendEvent<structData>(Message1,DataSent);

	  std::cout << "Thread Sender: sendEvent(Message1) nb:" << counter  << std::endl;
	}

	//boost::this_thread::sleep(workTime);
	std::cout << "Thread Sender: finished" << std::endl;
}

void ThreadSenderWithMessagesString(long nbMessages)
{
	long counter;
	std::string DataSent = "Hello World";

	std::cout << "Thread Sender: sendEvent (a)" << std::endl;
	for (counter = 0 ; counter < nbMessages; counter++)
	{
	  EvtHandler.sendEvent<std::string>(Message1,DataSent);

	  std::cout << "Thread Sender: sendEvent(Message1) nb:" << counter  << std::endl;
	}

	//boost::this_thread::sleep(workTime);
	std::cout << "Thread Sender: finished" << std::endl;
}

void InitReceiverCounter()
{
	ReceiverFirstMessageCounter = 0;
	ReceiverMessage1Counter     = 0;
	ReceiverTimeoutCounter      = 0;
	ReceiverDefaultCounter      = 0;
}

bool ValidateBadConversionException(shared::exception::CBadConversion exception)
{
   //return exception.isError();
	return true;
}

//--------------------------------------------------------------
/// \brief	    Test Events with 1 event sent - 1 Sender / 1 Receiver
/// \result         No Error
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Event_1_Frame)
{
	long nbOfFrames = 1;

	InitReceiverCounter();

	boost::thread receiverThread(ThreadReceiver , nbOfFrames);
	boost::thread senderThread(ThreadSender     , nbOfFrames);

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
	long nbOfFrames = 100;

	InitReceiverCounter();

	boost::thread receiverThread(ThreadReceiver , nbOfFrames);
	boost::thread senderThread(ThreadSender     , nbOfFrames);

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
	long nbOfFrames        = 1;
	ReceiverMessageCorrect = true;

	InitReceiverCounter();

	boost::thread receiverThread(ThreadReceiverWithMessages , nbOfFrames);
	boost::thread senderThread(ThreadSenderWithMessages     , nbOfFrames);

	receiverThread.join();

	BOOST_CHECK_EQUAL(ReceiverFirstMessageCounter, 0);
	BOOST_CHECK_EQUAL(ReceiverMessage1Counter    , nbOfFrames);
	BOOST_CHECK_EQUAL(ReceiverTimeoutCounter     , 0);
	BOOST_CHECK_EQUAL(ReceiverDefaultCounter     , 0);
	BOOST_CHECK_EQUAL(ReceiverMessageCorrect     , true);
}

//--------------------------------------------------------------
/// \brief	    Test Events with 1 event sent with Data - 1 Sender / 1 Receiver
/// \result         No Error
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Event_100_Frame_with_Data)
{
	long nbOfFrames        = 100;
	ReceiverMessageCorrect = true;

	InitReceiverCounter();

	boost::thread receiverThread(ThreadReceiverWithMessages , nbOfFrames);
	boost::thread senderThread(ThreadSenderWithMessages     , nbOfFrames);

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
	long nbOfFramesSend     = 0;
	long nbOfFramesReceived = 1;

	InitReceiverCounter();

	boost::thread receiverThread(ThreadReceiver , nbOfFramesReceived);
	boost::thread senderThread  (ThreadSender   , nbOfFramesSend    );

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
	long nbOfFramesSend     = 50;
	long nbOfFramesReceived = 100;

	InitReceiverCounter();

	boost::thread receiverThread (ThreadReceiver , nbOfFramesReceived);
	boost::thread senderThread1  (ThreadSender   , nbOfFramesSend    );
	boost::thread senderThread2  (ThreadSender   , nbOfFramesSend    );

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
	long nbOfFramesSend     = 1;
	long nbOfFramesReceived = 100;

	InitReceiverCounter();

	boost::thread receiverThread (ThreadReceiver , nbOfFramesReceived);

	for (char counter = 0; counter < nbOfFramesReceived; counter++)
	{
		boost::thread senderThread2  (ThreadSender   , nbOfFramesSend    );
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
	long nbOfFrames        = 1;
	ReceiverMessageCorrect = true;

	EvtHandler.sendEvent<std::string>(Message1,"Hello World");

	InitReceiverCounter();

	BOOST_REQUIRE_THROW (EvtHandler.popEvent<structData>(),shared::exception::CBadConversion);
}

BOOST_AUTO_TEST_SUITE_END()
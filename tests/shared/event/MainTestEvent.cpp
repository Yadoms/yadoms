// Includes needed to compile tested classes
#include <iostream>
#include <fstream>
#include <queue>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include "../../../sources/shared/shared/exceptions/BadConversion.hpp"
#include "../../../sources/shared/shared/event/EventHandler.hpp"

#define BOOST_TEST_MODULE MainTestEvent

#include <boost/test/unit_test.hpp>


// Event IDs
enum
{
   FirstMessage = shared::event::kUserFirstId,   // Always start from shared::event::kUserFirstId
   Message1
};

long Receiver_FirstMessageCounter;
long Receiver_Message1Counter;
long Receiver_TimeoutCounter;
long Receiver_DefaultCounter;

shared::event::CEventHandler EvtHandler;

void ThreadReceiver(long l_nb_messages)
{
   while ((Receiver_FirstMessageCounter + Receiver_Message1Counter + Receiver_TimeoutCounter + Receiver_DefaultCounter ) != l_nb_messages)
   {

      //boost::posix_time::seconds workTime(1);

      std::cout << "Thread Receiver: WaitForEvents()" << std::endl;
      //Pretend to do something useful...
      switch(EvtHandler.waitForEvents(boost::posix_time::milliseconds(10000)))
      {
      case FirstMessage:
         {
            EvtHandler.popEvent();
            Receiver_FirstMessageCounter++;
            std::cout << "Thread Receiver: First message received (#" << Receiver_FirstMessageCounter  << ")" << std::endl;
            break;
         }
      case Message1:
         {
            EvtHandler.popEvent();
            Receiver_Message1Counter++;
            std::cout << "Thread Receiver: Message 1 Received (#" << Receiver_Message1Counter << ")" << std::endl;
            break;
         }
      case shared::event::kTimeout:
         {
            Receiver_TimeoutCounter++;
            std::cout << "Thread Receiver: TimeOut (#" << Receiver_TimeoutCounter << ")" << std::endl;
            break;
         }
      default:
         {
            Receiver_DefaultCounter++;
            std::cout << "Thread Receiver: Unknown message id (#" << Receiver_DefaultCounter << ")" << std::endl;
            break;
         }
      }
   }
   std::cout << "Thread Receiver: finished" << std::endl;
}

void ThreadSender(long nb_messages)
{
	long i;

	//boost::posix_time::seconds workTime(1);
	//std::cout << "Thread Sender: sleep" << std::endl;

	//Pretend to do something useful...
	//boost::this_thread::sleep(workTime);
	std::cout << "Thread Sender: sendEvent" << std::endl;
	for (i = 0 ; i < nb_messages; i++)
	{
	  EvtHandler.sendEvent(Message1);

	  std::cout << "Thread Sender: sendEvent(Message1) nb:" << i  << std::endl;
	}

	//boost::this_thread::sleep(workTime);
	std::cout << "Thread Sender: finished" << std::endl;
}

//BOOST_AUTO_TEST_SUITE(Initialisation)

//--------------------------------------------------------------
/// \brief	    Test Events with 1 event sent
/// \result         No Error
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Event_1_Message)
{
	long l_nbmessages = 1;

	Receiver_FirstMessageCounter = 0;
	Receiver_Message1Counter     = 0;
	Receiver_TimeoutCounter      = 0;
	Receiver_DefaultCounter      = 0;

	boost::thread ConsoThread(ThreadReceiver , l_nbmessages);//TODO pas de français !
	boost::thread ProdThread(ThreadSender    , l_nbmessages);

	ConsoThread.join();

	BOOST_CHECK_EQUAL(Receiver_FirstMessageCounter, 0);
	BOOST_CHECK_EQUAL(Receiver_Message1Counter    , l_nbmessages);
	BOOST_CHECK_EQUAL(Receiver_TimeoutCounter     , 0);
	BOOST_CHECK_EQUAL(Receiver_DefaultCounter     , 0);
}

//--------------------------------------------------------------
/// \brief	    Test Events with 100 events sent simultaneously
/// \result         No Error
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Event_100_Messages)
{
	long l_nbmessages = 100;

	Receiver_FirstMessageCounter = 0;
	Receiver_Message1Counter     = 0;
	Receiver_TimeoutCounter      = 0;
	Receiver_DefaultCounter      = 0;

	boost::thread ConsoThread(ThreadReceiver , l_nbmessages);
	boost::thread ProdThread(ThreadSender    , l_nbmessages);

	ConsoThread.join();

	BOOST_CHECK_EQUAL(Receiver_FirstMessageCounter, 0);
	BOOST_CHECK_EQUAL(Receiver_Message1Counter    , l_nbmessages);
	BOOST_CHECK_EQUAL(Receiver_TimeoutCounter     , 0);
	BOOST_CHECK_EQUAL(Receiver_DefaultCounter     , 0);
}


//BOOST_AUTO_TEST_SUITE_END()

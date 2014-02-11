// \brief #define BOOST_AUTO_TEST_MAIN This definition is now configured into the CmakeFile
#include "stdafx.h"

/*
\file main.cpp

\brief Can use "brief" tag to explicitly generate comments for file documentation.

*/

#include <iostream>
#include <fstream>
#include <queue>
#include <shared/Exceptions/BadConversionException.hpp>
#include <shared/Event/Event.hpp>
#include <shared/Event/EventBase.hpp>
#include <shared/Event/EventHandler.hpp>

#define _DEBUG
//#define BOOST_TEST_MODULE MainTestEvent

#include <boost/test/unit_test.hpp>

/* TODO : A voir pour sortie directe en xml
#include <boost/test/results_reporter.hpp>

//--------------------------------------------------------------
/// \brief	    Redirect in an xml file the result
//--------------------------------------------------------------

std::ofstream out;

struct ReportRedirector
{
    ReportRedirector()
    {
        out.open("test_results.xml");
        assert( out.is_open() );
        boost::unit_test::results_reporter::set_stream(out);
    }
};

BOOST_GLOBAL_FIXTURE(ReportRedirector)
*/

/*
\brief Main application entry point
*/

// Event IDs

enum
{
   FirstMessage = CEventHandler::kUserFirstId,   // Always start from CEventHandler::kUserFirstId
   Message1
};

long Receiver_FirstMessageCounter;
long Receiver_Message1Counter;
long Receiver_TimeoutCounter;
long Receiver_DefaultCounter;

CEventHandler EvtHandler;

void ThreadReceiver(long l_nb_messages)
{
	while ((Receiver_FirstMessageCounter + Receiver_TimeoutCounter + Receiver_DefaultCounter ) != l_nb_messages)
	{

		//boost::posix_time::seconds workTime(1);

#ifdef _DEBUG
		std::cout << "Thread Receiver: WaitForEvents()" << std::endl;
#endif
	//Pretend to do something useful...
 	        switch(EvtHandler.waitForEvents(boost::posix_time::milliseconds(10000)))
          	{
		  case FirstMessage:
		    {
		        EvtHandler.popEvent();
			Receiver_FirstMessageCounter++;
#ifdef _DEBUG
	               std::cout << "Thread Receiver: First message received..." << std::endl;
#endif
		       break;
		    }
		 case Message1:
		    {
		        EvtHandler.popEvent();
			Receiver_Message1Counter++;
#ifdef _DEBUG
	               std::cout << "Thread Receiver: Message 1 Received..." << std::endl;
#endif
		       break;
		    }
		 case CEventHandler::kTimeout:
		    {
		       Receiver_TimeoutCounter++;
#ifdef _DEBUG
	               std::cout << "Thread Receiver: TimeOut..." << std::endl;
#endif
		       break;
		    }
		 default:
		    {
			Receiver_DefaultCounter++;
#ifdef _DEBUG
			std::cout << "Thread Receiver: Unknown message id..." << std::endl;
#endif
		       break;
		    }
		}
   }
#ifdef _DEBUG
	std::cout << "Thread Receiver: finished" << std::endl;
#endif
}

void ThreadSender(long nb_messages)
{
	long i;

	//boost::posix_time::seconds workTime(1);
#ifdef _DEBUG
	//std::cout << "Thread Sender: sleep" << std::endl;
#endif

	//Pretend to do something useful...
	//boost::this_thread::sleep(workTime);
#ifdef _DEBUG
	std::cout << "Thread Sender: sendEvent" << std::endl;
#endif
	for (i = 0 ; i < nb_messages; i++)
	{
	  EvtHandler.sendEvent(Message1);

#ifdef _DEBUG
	  std::cout << "Thread Sender: sendEvent(Message1) nb:" << i  << std::endl;
#endif
	}

	//boost::this_thread::sleep(workTime);
#ifdef _DEBUG
	std::cout << "Thread Sender: finished" << std::endl;
#endif
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

	boost::thread ConsoThread(ThreadReceiver , l_nbmessages);
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

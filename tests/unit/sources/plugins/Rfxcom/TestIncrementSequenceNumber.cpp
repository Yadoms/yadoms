#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/Rfxcom/IncrementSequenceNumber.h"

BOOST_AUTO_TEST_SUITE(TestIncrementSequenceNumber)

   BOOST_AUTO_TEST_CASE(Nominal)
   {
      CIncrementSequenceNumber seqNumber;

      BOOST_CHECK_EQUAL(seqNumber.currentToSend(), 0);
      BOOST_CHECK(seqNumber.checkReceived(0));

      BOOST_CHECK_EQUAL(seqNumber.nextToSend(), 1);
      BOOST_CHECK_EQUAL(seqNumber.currentToSend(), 1);
      BOOST_CHECK(seqNumber.checkReceived(1));

      BOOST_CHECK_EQUAL(seqNumber.nextToSend(), 2);
      BOOST_CHECK_EQUAL(seqNumber.currentToSend(), 2);
      BOOST_CHECK(seqNumber.checkReceived(2));

      BOOST_CHECK_EQUAL(seqNumber.nextToSend(), 3);
      BOOST_CHECK_EQUAL(seqNumber.currentToSend(), 3);
      BOOST_CHECK(seqNumber.checkReceived(3));
   }

   BOOST_AUTO_TEST_CASE(QuickSend)
   {
      CIncrementSequenceNumber seqNumber;

      BOOST_CHECK_EQUAL(seqNumber.currentToSend(), 0);

      BOOST_CHECK_EQUAL(seqNumber.nextToSend(), 1);
      BOOST_CHECK_EQUAL(seqNumber.nextToSend(), 2);
      BOOST_CHECK_EQUAL(seqNumber.nextToSend(), 3);

      BOOST_CHECK(seqNumber.checkReceived(3));
   }
   
   BOOST_AUTO_TEST_CASE(Resynchronize)
   {
      CIncrementSequenceNumber seqNumber; 

      BOOST_CHECK_EQUAL(seqNumber.currentToSend(), 0);
      BOOST_CHECK(seqNumber.checkReceived(0));

      BOOST_CHECK_EQUAL(seqNumber.nextToSend(), 1);
      BOOST_CHECK(seqNumber.checkReceived(1));

      BOOST_CHECK_EQUAL(seqNumber.nextToSend(), 2);
      BOOST_CHECK(seqNumber.checkReceived(2));

      BOOST_CHECK_EQUAL(seqNumber.nextToSend(), 3);
      BOOST_CHECK(seqNumber.checkReceived(3));

      seqNumber.reset(58);
      BOOST_CHECK_EQUAL(seqNumber.currentToSend(), 58);
      BOOST_CHECK(seqNumber.checkReceived(58));

      BOOST_CHECK_EQUAL(seqNumber.nextToSend(), 59);
      BOOST_CHECK_EQUAL(seqNumber.currentToSend(), 59);
      BOOST_CHECK(seqNumber.checkReceived(59));

      BOOST_CHECK_EQUAL(seqNumber.nextToSend(), 60);
      BOOST_CHECK_EQUAL(seqNumber.currentToSend(), 60);

      BOOST_CHECK_EQUAL(seqNumber.nextToSend(), 61);
      BOOST_CHECK_EQUAL(seqNumber.currentToSend(), 61);

      BOOST_CHECK(seqNumber.checkReceived(61));
   }


BOOST_AUTO_TEST_SUITE_END()

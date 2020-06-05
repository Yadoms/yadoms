#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/Rfxcom/IncrementSequenceNumber.h"

BOOST_AUTO_TEST_SUITE(TestIncrementSequenceNumber)

BOOST_AUTO_TEST_CASE(Nominal)
{
   CIncrementSequenceNumber seqNumber;

   BOOST_CHECK_EQUAL(seqNumber.last(), 0);
   BOOST_CHECK(seqNumber.isExpected(0));

   BOOST_CHECK_EQUAL(seqNumber.next(), 1);
   BOOST_CHECK_EQUAL(seqNumber.last(), 1);
   BOOST_CHECK(seqNumber.isExpected(1));

   BOOST_CHECK_EQUAL(seqNumber.next(), 2);
   BOOST_CHECK_EQUAL(seqNumber.last(), 2);
   BOOST_CHECK(seqNumber.isExpected(2));

   BOOST_CHECK_EQUAL(seqNumber.next(), 3);
   BOOST_CHECK_EQUAL(seqNumber.last(), 3);
   BOOST_CHECK(seqNumber.isExpected(3));
}

BOOST_AUTO_TEST_CASE(QuickSend)
{
   CIncrementSequenceNumber seqNumber;

   BOOST_CHECK_EQUAL(seqNumber.last(), 0);

   BOOST_CHECK_EQUAL(seqNumber.next(), 1);
   BOOST_CHECK_EQUAL(seqNumber.next(), 2);
   BOOST_CHECK_EQUAL(seqNumber.next(), 3);

   BOOST_CHECK(seqNumber.isExpected(3));
}

BOOST_AUTO_TEST_CASE(Resynchronize)
{
   CIncrementSequenceNumber seqNumber;

   BOOST_CHECK_EQUAL(seqNumber.last(), 0);
   BOOST_CHECK(seqNumber.isExpected(0));

   BOOST_CHECK_EQUAL(seqNumber.next(), 1);
   BOOST_CHECK(seqNumber.isExpected(1));

   BOOST_CHECK_EQUAL(seqNumber.next(), 2);
   BOOST_CHECK(seqNumber.isExpected(2));

   BOOST_CHECK_EQUAL(seqNumber.next(), 3);
   BOOST_CHECK(seqNumber.isExpected(3));

   seqNumber.reset(58);
   BOOST_CHECK_EQUAL(seqNumber.last(), 58);
   BOOST_CHECK(seqNumber.isExpected(58));

   BOOST_CHECK_EQUAL(seqNumber.next(), 59);
   BOOST_CHECK_EQUAL(seqNumber.last(), 59);
   BOOST_CHECK(seqNumber.isExpected(59));

   BOOST_CHECK_EQUAL(seqNumber.next(), 60);
   BOOST_CHECK_EQUAL(seqNumber.last(), 60);

   BOOST_CHECK_EQUAL(seqNumber.next(), 61);
   BOOST_CHECK_EQUAL(seqNumber.last(), 61);

   BOOST_CHECK(seqNumber.isExpected(61));
}


BOOST_AUTO_TEST_SUITE_END()

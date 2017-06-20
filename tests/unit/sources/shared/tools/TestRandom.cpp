#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include <iostream>
#include <queue>
#include "../../../../sources/shared/shared/tools/Random.h"

BOOST_AUTO_TEST_SUITE(TestRandom)


//--------------------------------------------------------------
/// \brief	    Test generate Nb bits
/// \result         No Error
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(CheckGenerateNbBits)
{
   static const unsigned int nbValues = 100;
   std::set<unsigned int> values;
   for (unsigned int i=0; i<nbValues; ++i)
   {
      auto newValue = shared::tools::CRandom::generateNbBits(16, false);
      values.insert(newValue);
   }
   // Several times same value is possible, but if same value is seen more than 3 times, we consider that
   // random generator doesn't work
   BOOST_CHECK_GT(values.size(), nbValues - 3);
}

BOOST_AUTO_TEST_SUITE_END()
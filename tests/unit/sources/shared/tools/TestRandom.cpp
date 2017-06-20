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
BOOST_AUTO_TEST_CASE(GenerateNbBits)
{
   static const unsigned int nbValues = 100;
   std::set<unsigned int> values;
   for (int i=0; i<nbValues; ++i)
   {
      auto newValue = shared::tools::CRandom::generateNbBits(16, false);
      values.insert(newValue);
   }
   BOOST_CHECK_EQUAL(values.size(), nbValues);
}

BOOST_AUTO_TEST_SUITE_END()
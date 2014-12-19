#include "stdafx.h"
#include "Random.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

namespace shared {
   namespace tools {

      unsigned int CRandom::generate(int min, int max)
      {
         boost::random::mt19937 gen;
         boost::random::uniform_int_distribution<> dist(min, max);
         return dist(gen);
      }

      unsigned int CRandom::generate(int max)
      {
         return generate(0, max);
      }  
      
      unsigned int CRandom::generate()
      {
         boost::random::mt19937 gen;
         boost::random::uniform_int_distribution<> dist;
         return dist(gen);
      }

   } // namespace tools 
} // namespace shared
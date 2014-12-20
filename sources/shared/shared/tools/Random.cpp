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
      
		unsigned int CRandom::generate(bool includeZero)
      {
         boost::random::mt19937 gen;
         boost::random::uniform_int_distribution<> dist;
			if (includeZero)
				return dist(gen);
			else
			{
				unsigned int nonzero = dist(gen);
				while (nonzero == 0)
					nonzero = dist(gen);
				return nonzero;
			}
      }

		unsigned int CRandom::generateNbBits(int n, bool includeZero)
		{
			return generate(includeZero ? 0 : 1, (int)pow(2, n) - 1);
		}
   } // namespace tools 
} // namespace shared
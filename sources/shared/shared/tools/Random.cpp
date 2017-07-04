#include "stdafx.h"
#include "Random.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <Poco/UUID.h>
#include <Poco/UUIDGenerator.h>

namespace shared
{
   namespace tools
   {
      static boost::random::mt19937& Generator()
      {
         static boost::random::mt19937 Generator;
         return Generator;
      }

      int CRandom::generate(int min, int max)
      {
         boost::random::uniform_int_distribution<> dist(min, max);
         return dist(Generator());
      }

      int CRandom::generate(int max)
      {
         return generate(0, max);
      }

      int CRandom::generate(bool includeZero)
      {
         boost::random::mt19937 gen;
         boost::random::uniform_int_distribution<> dist;
         if (includeZero)
            return dist(gen);
         
         unsigned int nonzero = dist(gen);
         while (nonzero == 0)
            nonzero = dist(gen);
         return nonzero;
      }

      unsigned int CRandom::generateNbBits(int n, bool includeZero)
      {
         return static_cast<unsigned int>(generate(includeZero ? 0 : 1,
                                                   static_cast<int>(pow(2.0, n) - 1)));
      }


      std::string CRandom::generateUUID()
      {
         auto& generator = Poco::UUIDGenerator::defaultGenerator();
         auto uuid(generator.createRandom());
         return uuid.toString();
      }
   } // namespace tools 
} // namespace shared



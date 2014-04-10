#pragma once

namespace tools
{

   //---------------------------------------------
   ///\brief Class which simplify random generation
   //---------------------------------------------
   class CRandom
   {
   public:

      //---------------------------------------------
      ///\brief Generate a random number using a nu:ber of random bits (from 1 to 32)
      ///\param [in] zeroAllowed If true the value 0 may be returns, else never
      ///\return a random integer ni the range [0; (2^NbBits) -1]
      ///\template NbBits the number of independant bits in random generator
      //---------------------------------------------
      template<size_t NbBits>
      static unsigned int generateRandomNumber(bool zeroAllowed)
      {
         BOOST_ASSERT(NbBits > 0);
         BOOST_ASSERT(NbBits <= 32);

         boost::random::independent_bits_engine<boost::random::mt19937, NbBits, boost::uint32_t> genbits((unsigned int)time(0));
         boost::random::uniform_int_distribution<> m_dist(zeroAllowed?0:1, ((int)pow(2,NbBits))-1);
         return m_dist(genbits); 
      }      


   private:
      //---------------------------------------------
      ///\brief Contructor
      //---------------------------------------------
      CRandom()
      {
      }

      //---------------------------------------------
      ///\brief Destructor
      //---------------------------------------------
      virtual ~CRandom()
      {
      }

   };

} // namespace tools
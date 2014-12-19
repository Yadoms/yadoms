#pragma once
#include <shared/Export.h>

namespace shared { namespace tools {

   //---------------------------------------------
   ///\brief Class which simplify random generation
   //---------------------------------------------
   class YADOMS_SHARED_EXPORT CRandom
   {
   public:
      //---------------------------------------------
      ///\brief Generate a random number between min and max (included)
      ///\param [in] min  The minimum value (included)
      ///\param [in] max  The maximum value (included)
      ///\return a random integer ni the range [min; max]
      ///\template NbBits the number of independent bits in random generator
      //---------------------------------------------
      static unsigned int generate(int min, int max);

      //---------------------------------------------
      ///\brief Generate a random number between 0 and max (included)
      ///\param [in] max  The maximum value (included)
      ///\return a random integer ni the range [0; max]
      ///\template NbBits the number of independent bits in random generator
      //---------------------------------------------
      static unsigned int generate(int max);    
      
      //---------------------------------------------
      ///\brief Generate a random number between 0 and 2^32
      ///\return a random integer ni the range [0; 2^32]
      ///\template NbBits the number of independent bits in random generator
      //---------------------------------------------
      static unsigned int generate();
   };

} // namespace tools 
} // namespace shared
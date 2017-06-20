#pragma once
#include <shared/Export.h>

namespace shared
{
	namespace tools
	{
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
			///\param [in] includeZero  If true zero as return value is allowed
			///\return a random integer ni the range [ (0 or 1) ; 2^32]
			//---------------------------------------------
			static unsigned int generate(bool includeZero = false);

			//---------------------------------------------
			///\brief Generate a random number between 0 and 2^n (generate for n bits)
			///\param [in] n  The power of 2
			///\return a random integer ni the range [ (0 or 1) ; 2^n]
			//---------------------------------------------
			static unsigned int generateNbBits(int n, bool includeZero = false);

			//---------------------------------------------
			///\brief Generate a random UUID string  : ie. : "3695d8ab-05d9-4ad0-8d74-61225df7139b"
			///\return a random UUID string
			//---------------------------------------------
			static std::string generateUUID();
		};
	} // namespace tools 
} // namespace shared



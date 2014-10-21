#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{
   //-----------------------------------------------------
   ///\brief A rssi (signal strength) historizable object
   ///\note Rssi value is read-only
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CRssi : public CSingleHistorizableData<int>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] measureType     The measure type (normally kAbsolute)
      //-----------------------------------------------------
      CRssi(const std::string& keywordName, const EMeasureType& measureType = EMeasureType::kAbsolute);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CRssi();

   protected:
      //-----------------------------------------------------
      ///\brief                     Normalize the value
      ///\param[in] level           Raw value
      ///\return                    The normalized value (0-100)
      //-----------------------------------------------------
      virtual int Normalize(int value);

   };



} } } } // namespace shared::plugin::yadomsApi::historization


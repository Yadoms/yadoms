#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"


namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{   
   //-----------------------------------------------------
   ///\brief A rain historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CRainRate : public CSingleHistorizableData<double>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] measureType     The measure type (normally kCumulative)
      //-----------------------------------------------------
      CRainRate(const std::string& keywordName, const EMeasureType& measureType = EMeasureType::kCumulative);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CRainRate();
   };



} } } } // namespace shared::plugin::yadomsApi::historization


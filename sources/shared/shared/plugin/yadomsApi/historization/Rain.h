#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"


namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{   
   //-----------------------------------------------------
   ///\brief A rain historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CRain : public CSingleHistorizableData<double>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] measureType     The measure type (normally kCumulative)
      //-----------------------------------------------------
      CRain(const std::string& keywordName, const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet, const EMeasureType& measureType = EMeasureType::kCumulative);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CRain();
   };



} } } } // namespace shared::plugin::yadomsApi::historization


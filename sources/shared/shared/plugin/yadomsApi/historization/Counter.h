#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{   
   //-----------------------------------------------------
   ///\brief A temperature historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CCounter : public CSingleHistorizableData<int>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] measureType     To be used as increment counter (values will be added to current database value) or totalizer
      //-----------------------------------------------------
      CCounter(const std::string& keywordName, const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet, const EMeasureType& measureType = EMeasureType::kCumulative);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CCounter();

   };



} } } } // namespace shared::plugin::yadomsApi::historization


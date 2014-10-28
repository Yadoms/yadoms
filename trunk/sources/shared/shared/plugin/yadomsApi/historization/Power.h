#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"


namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{   
   //-----------------------------------------------------
   ///\brief A power historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CPower : public CSingleHistorizableData<double>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] measureType     The measure type (normally kAbsolute)
      //-----------------------------------------------------
      CPower(const std::string& keywordName, const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet, const EMeasureType& measureType = EMeasureType::kAbsolute);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CPower();

   };



} } } } // namespace shared::plugin::yadomsApi::historization


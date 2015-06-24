#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   //-----------------------------------------------------
   ///\brief A distance (meter) historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CDistance : public CSingleHistorizableData < double >
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      Access mode
      //-----------------------------------------------------
      CDistance(const std::string& keywordName, const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CDistance();
   };

} } } } // namespace shared::plugin::yPluginApi::historization

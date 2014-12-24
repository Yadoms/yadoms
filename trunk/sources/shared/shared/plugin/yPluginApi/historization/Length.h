#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   //-----------------------------------------------------
   ///\brief A length (meter) historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CLength : public CSingleHistorizableData < double >
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      Access mode
      //-----------------------------------------------------
      CLength(const std::string& keywordName, const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CLength();
   };

} } } } // namespace shared::plugin::yPluginApi::historization

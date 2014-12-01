#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{
   //-----------------------------------------------------
   ///\brief A load (%) historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CLoad : public CSingleHistorizableData < double >
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      Access mode
      //-----------------------------------------------------
      CLoad(const std::string& keywordName, const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CLoad();
   };

} } } } // namespace shared::plugin::yadomsApi::historization
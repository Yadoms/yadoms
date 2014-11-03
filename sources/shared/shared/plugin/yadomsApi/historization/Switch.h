#pragma once
#include <shared/Export.h>
#include <shared/plugin/yadomsApi/historization/SingleHistorizableData.hpp>

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{   
   //-----------------------------------------------------
   ///\brief A switch historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CSwitch : public CSingleHistorizableData<bool>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      The access mode
      //-----------------------------------------------------
      CSwitch(const std::string& keywordName, const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGetSet);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CSwitch();
   };



} } } } // namespace shared::plugin::yadomsApi::historization


#pragma once
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include "SingleHistorizableData.hpp"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief Plugin state
            //-----------------------------------------------------
            DECLARE_ENUM_HEADER_SHARED(EPluginState, YADOMS_SHARED_EXPORT,
               ((Unknown))
               ((Error))
               ((Stopped))
               ((Running))
               ((Custom))
            )


            //-----------------------------------------------------
            ///\brief The plugin state historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CPluginState : public CSingleHistorizableData<EPluginState>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               //-----------------------------------------------------
               explicit CPluginState(const std::string& keywordName,
                                     const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet);

               //-----------------------------------------------------
               ///\brief                     Destructor
               //-----------------------------------------------------
               virtual ~CPluginState();
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization



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
            ///\brief Device state
            //-----------------------------------------------------
            DECLARE_ENUM_HEADER_SHARED(EDeviceState, YADOMS_SHARED_EXPORT,
               ((Unknown))
               ((Active))
               ((Busy))
               ((Asleep))
               ((Dead))
               ((Custom))
            )


            //-----------------------------------------------------
            ///\brief The device state historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CDeviceState : public CSingleHistorizableData<EDeviceState>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CDeviceState(const std::string& keywordName,
                                     const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                     const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CDeviceState() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization

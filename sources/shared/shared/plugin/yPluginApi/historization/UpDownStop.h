#pragma once
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
            ///\brief               The curtain command values
            //-----------------------------------------------------   
            DECLARE_ENUM_HEADER(EUpDownStopCommand,
               ((Stop))
               ((Up))
               ((Down))
            ) ;


            //-----------------------------------------------------
            ///\brief A curtain historizable object
            //-----------------------------------------------------
            class CUpDownStop : public CSingleHistorizableData<EUpDownStopCommand>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               CUpDownStop(const std::string& keywordName,
                           const EKeywordAccessMode& accessMode,
                           const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CUpDownStop() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization

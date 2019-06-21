#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"
#include <Poco/Types.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief A color (RGB) historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CColorRGB : public CSingleHistorizableData<Poco::UInt32>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      Access mode
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               CColorRGB(const std::string& keywordName,
                         const EKeywordAccessMode& accessMode,
                         const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CColorRGB() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization

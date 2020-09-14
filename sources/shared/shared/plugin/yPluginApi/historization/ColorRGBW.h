#pragma once
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
            ///\brief A color historizable object
            //-----------------------------------------------------
            class CColorRGBW : public CSingleHistorizableData<Poco::UInt32>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      Access mode
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               CColorRGBW(const std::string& keywordName,
                          const EKeywordAccessMode& accessMode,
                          const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               void setRGBW(int red, int green, int blue, int white);

               virtual ~CColorRGBW() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization

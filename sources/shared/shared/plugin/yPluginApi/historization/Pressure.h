#pragma once
#include "SingleHistorizableData.hpp"
#include "../typeInfo/DoubleTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief A pressure historizable object
            //-----------------------------------------------------
            class CPressure : public CSingleHistorizableData<double>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] measureType     The measure type (normally kAbsolute)
               ///\param[in] additionalInfo  The type information
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CPressure(const std::string& keywordName,
                                  const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                  const EMeasureType& measureType = EMeasureType::kAbsolute,
                                  const typeInfo::CDoubleTypeInfo& additionalInfo = typeInfo::CDoubleTypeInfo::Empty,
                                  const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CPressure() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization

#pragma once
#include "SingleHistorizableData.hpp"
#include "../typeInfo/IntTypeInfo.h"
#include "shared/plugin/yPluginApi/typeInfo/DoubleTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief A direction historizable object
            //-----------------------------------------------------
            class CDirection : public CSingleHistorizableData<int>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] measureType     The measure type (normally kAbsolute)
               ///\param[in] additionalInfo  Additional information
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CDirection(const std::string& keywordName,
                                   const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                   const EMeasureType& measureType = EMeasureType::kAbsolute,
                                   const typeInfo::CIntTypeInfo& additionalInfo = typeInfo::CIntTypeInfo::Empty,
                                   const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CDirection() = default;
            };

            //-----------------------------------------------------
            ///\brief A direction historizable object
            //-----------------------------------------------------
            class CDirectionDouble : public CSingleHistorizableData<double>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] measureType     The measure type (normally kAbsolute)
               ///\param[in] additionalInfo  Additional information
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CDirectionDouble(const std::string& keywordName,
                  const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                  const EMeasureType& measureType = EMeasureType::kAbsolute,
                  const typeInfo::CDoubleTypeInfo& additionalInfo = typeInfo::CDoubleTypeInfo::Empty,
                  const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CDirectionDouble() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization

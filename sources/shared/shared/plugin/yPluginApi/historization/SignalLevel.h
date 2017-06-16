#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"
#include "typeInfo/IntTypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief A signal Level (unit : dB) historizable object
            ///\note signal Level value is read-only
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CSignalLevel : public CSingleHistorizableData<int>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] measureType     The measure type (normally kAbsolute)
               //-----------------------------------------------------
               explicit CSignalLevel(const std::string& keywordName,
                                 const EMeasureType& measureType = EMeasureType::kAbsolute,
                                 typeInfo::CIntTypeInfo& additionalInfo = typeInfo::CIntTypeInfo::Empty);

               //-----------------------------------------------------
               ///\brief                     Destructor
               //-----------------------------------------------------
               virtual ~CSignalLevel();
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization



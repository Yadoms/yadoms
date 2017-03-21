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
            ///\brief A battery level historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CBatteryLevel : public CSingleHistorizableData<int>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] measureType     The measure type (normally kAbsolute)
               //-----------------------------------------------------
               explicit CBatteryLevel(const std::string& keywordName,
                                      const EMeasureType& measureType = EMeasureType::kAbsolute,
                                      typeInfo::CIntTypeInfo& additionalInfo = typeInfo::CIntTypeInfo::Empty);

               //-----------------------------------------------------
               ///\brief                     Destructor
               //-----------------------------------------------------
               virtual ~CBatteryLevel();

            protected:
               //-----------------------------------------------------
               ///\brief                     Normalize the battery level [0; 100]
               ///\param[in] value           Raw value
               ///\return                    The normalized value [0; 100]
               //-----------------------------------------------------
               int Normalize(int value) override;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization



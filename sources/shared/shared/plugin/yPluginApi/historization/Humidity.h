#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"
#include "typeInfo/DoubleTypeInfo.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief A humidity historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CHumidity : public CSingleHistorizableData<double>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] measureType     The measure type (normally kAbsolute)
               //-----------------------------------------------------
               explicit CHumidity(const std::string& keywordName,
                                  const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                  const EMeasureType& measureType = EMeasureType::kAbsolute,
                                  typeInfo::CDoubleTypeInfo& additionalInfo = typeInfo::CDoubleTypeInfo::Empty);

               //-----------------------------------------------------
               ///\brief                     Destructor
               //-----------------------------------------------------
               virtual ~CHumidity();

            protected:
               //-----------------------------------------------------
               ///\brief                     Normalize the humidity [0; 100]
               ///\param[in] value           Raw value
               ///\return                    The normalized value [0; 100]
               //-----------------------------------------------------
               double Normalize(double value) override;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization



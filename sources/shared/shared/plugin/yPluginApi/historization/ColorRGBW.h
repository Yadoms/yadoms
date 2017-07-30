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
            ///\brief A color historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CColorRGBW : public CSingleHistorizableData<Poco::UInt32>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      Access mode
               //-----------------------------------------------------
               CColorRGBW(const std::string& keywordName,
                        const EKeywordAccessMode& accessMode);

               //-----------------------------------------------------
               ///\brief                     Destructor
               //-----------------------------------------------------
               virtual ~CColorRGBW();
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization



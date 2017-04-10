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
            ///\brief               The curtain command values
            //-----------------------------------------------------   
            DECLARE_ENUM_HEADER_SHARED(ECurtainCommand, YADOMS_SHARED_EXPORT,
               ((Stop))
               ((Open))
               ((Close))
            );


            //-----------------------------------------------------
            ///\brief A curtain historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CCurtain : public CSingleHistorizableData<ECurtainCommand>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               //-----------------------------------------------------
               explicit CCurtain(const std::string& keywordName);

               //-----------------------------------------------------
               ///\brief                     Destructor
               //-----------------------------------------------------
               virtual ~CCurtain();
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization

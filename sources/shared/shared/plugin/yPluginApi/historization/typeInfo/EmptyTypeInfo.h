#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include "ITypeInfo.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            namespace typeInfo
            {
               //-----------------------------------------------------
               ///\brief Interface for keyword type information
               //-----------------------------------------------------
               class YADOMS_SHARED_EXPORT CEmptyTypeInfo : public ITypeInfo
               {
               public:
                  CEmptyTypeInfo();
                  virtual ~CEmptyTypeInfo();
                  CDataContainer serialize() const override;

                  //-----------------------------------------------------
                  ///\brief   Empty type info
                  //-----------------------------------------------------
                  static CEmptyTypeInfo Empty;
               };
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo



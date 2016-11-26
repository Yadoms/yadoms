#pragma once
#include "ITypeInfo.h"
#include <shared/Export.h>
#include <shared/Field.hpp>

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
               class YADOMS_SHARED_EXPORT CBoolTypeInfo : public ITypeInfo
               {
               public:
                  //-----------------------------------------------------
                  ///\brief   Empty container (can be with references)
                  //-----------------------------------------------------
                  static CBoolTypeInfo Empty;

                  //-----------------------------------------------------
                  ///\brief   Constructor
                  //-----------------------------------------------------
                  CBoolTypeInfo();

                  //-----------------------------------------------------
                  ///\brief   Destructor
                  //-----------------------------------------------------
                  virtual ~CBoolTypeInfo();

                  // ITypeInfo implementation 
                  CDataContainer serialize() const override;
                  // END ITypeInfo implementation 
               };
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo



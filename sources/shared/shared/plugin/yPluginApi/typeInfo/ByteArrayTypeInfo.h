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
         namespace typeInfo
         {
            //-----------------------------------------------------
            ///\brief Interface for keyword type information
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CByteArrayTypeInfo : public ITypeInfo
            {
            public:
               //-----------------------------------------------------
               ///\brief   Empty container (can be with references)
               //-----------------------------------------------------
               static const CByteArrayTypeInfo Empty;

               CByteArrayTypeInfo() = default;
               virtual ~CByteArrayTypeInfo() = default;

               CByteArrayTypeInfo& setMaxLength(const int maxLength);

               // ITypeInfo implementation 
               CDataContainer serialize() const override;
               // END ITypeInfo implementation 

            private:
               CField<int> m_maxLength;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo

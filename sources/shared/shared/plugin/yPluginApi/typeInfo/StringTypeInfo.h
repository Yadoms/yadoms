#pragma once
#include "ITypeInfo.h"
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
            class CStringTypeInfo : public ITypeInfo
            {
            public:
               //-----------------------------------------------------
               ///\brief   Empty container (can be with references)
               //-----------------------------------------------------
               static const CStringTypeInfo Empty;

               CStringTypeInfo() = default;
               virtual ~CStringTypeInfo() = default;

               //-----------------------------------------------------
               ///\brief   Set the regex string
               ///\param [in] regexString : The regex string
               ///\return  A reference to itself to allow method chaining
               //-----------------------------------------------------
               CStringTypeInfo& setRegexString(const std::string& regexString);

               // ITypeInfo implementation 
               boost::shared_ptr<CDataContainer> serialize() const override;
               // END ITypeInfo implementation 

            private:
               CField<std::string> m_regex;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo

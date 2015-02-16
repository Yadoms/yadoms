#pragma once
#include "ITypeInfo.h"
#include <shared/Export.h>
#include <shared/Field.hpp>

namespace shared { namespace plugin { namespace yPluginApi { namespace historization { namespace typeInfo {

   //-----------------------------------------------------
   ///\brief Interface for keyword type information
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CStringTypeInfo : public ITypeInfo
   {
   public:
      //-----------------------------------------------------
      ///\brief   Empty container (can be with references)
      //-----------------------------------------------------
      static CStringTypeInfo Empty;
      
      //-----------------------------------------------------
      ///\brief   Constructor
      //-----------------------------------------------------
      CStringTypeInfo();
      
      //-----------------------------------------------------
      ///\brief   Destructor
      //-----------------------------------------------------
      virtual ~CStringTypeInfo();
      
      //-----------------------------------------------------
      ///\brief   Set the regex string
      ///\param [in] regexString : The regex string
      ///\return  A reference to itself to allow method chaining
      //-----------------------------------------------------
      CStringTypeInfo & setRegexString(const std::string & regexString);

      // ITypeInfo implementation 
      virtual shared::CDataContainer CStringTypeInfo::serialize() const;
      // END ITypeInfo implementation 

   private:
      CField<std::string> m_regex;
   };

}}}}} // namespace shared::plugin::yPluginApi::historization::typeInfo

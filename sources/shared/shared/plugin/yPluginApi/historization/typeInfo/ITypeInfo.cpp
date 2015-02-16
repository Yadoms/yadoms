#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>

namespace shared { namespace plugin { namespace yPluginApi { namespace historization { namespace typeInfo {

   //-----------------------------------------------------
   ///\brief Interface for keyword type information
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT ITypeInfo
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~ITypeInfo() {}

      //-----------------------------------------------------
      ///\brief   Serialize
      ///\return  A data container of the typeInfo
      //-----------------------------------------------------
      virtual CDataContainer serialize() const = 0;
   };

   
}}}}} // namespace shared::plugin::yPluginApi::historization::typeInfo
	
	
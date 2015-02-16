#include "stdafx.h"
#include "EmptyTypeInfo.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization { namespace typeInfo {


   CEmptyTypeInfo CEmptyTypeInfo::Empty;


   CEmptyTypeInfo::CEmptyTypeInfo()
   {
   }

   CEmptyTypeInfo::~CEmptyTypeInfo()
   {
   }

   CDataContainer CEmptyTypeInfo::serialize() const
   {
      return CDataContainer();
   }

}}}}} // namespace shared::plugin::yPluginApi::historization::typeInfo
	
	
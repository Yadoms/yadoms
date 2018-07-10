#pragma once
#include "../OpenZWaveSingleHistorizableData.h"
#include "../typeinfo/ByteArrayTypeInfo.h"
#include <shared/plugin/yPluginApi/StandardCapacity.h>

namespace historizers
{
   class CUserCode : public COpenZWaveSingleHistorizableData<std::string>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] ti              The measure type information
      //-----------------------------------------------------
      CUserCode(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CByteArrayTypeInfo &ti);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CUserCode();
   };
} //namespace historizers 



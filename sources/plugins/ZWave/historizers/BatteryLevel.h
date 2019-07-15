#pragma once
#include "../OpenZWaveSingleHistorizableData.h"
#include "../typeinfo/IntegerTypeInfo.h"

namespace historizers
{
   class CBatteryLevel : public COpenZWaveSingleHistorizableData<int>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] name            Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] ti              The measure type information
      //-----------------------------------------------------
      CBatteryLevel(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, CIntegerTypeInfo &ti);

      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] name            Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] measureType     The measure type (normally kAbsolute)
      ///\param[in] ti              The measure type information
      //-----------------------------------------------------
      CBatteryLevel(const std::string& name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, shared::plugin::yPluginApi::EMeasureType measureType, CIntegerTypeInfo &ti);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CBatteryLevel();
   };
} //namespace historizers 



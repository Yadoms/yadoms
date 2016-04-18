#include "stdafx.h"
#include "Switch.h"
#include <shared/plugin/yPluginApi/historization/Switch.h>

namespace historizers {

   //-----------------------------------------------------
   ///\brief                     Constructor
   ///\param[in] keywordName     Yadoms keyword name
   ///\param[in] accessMode      The access mode
   ///\param[in] measureType     The measure type (normally kAbsolute)
   //-----------------------------------------------------
   CSwitch::CSwitch(const std::string & name, shared::plugin::yPluginApi::EKeywordAccessMode accessMode)
      : COpenZWaveSingleHistorizableData<bool>(boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<bool> >(new shared::plugin::yPluginApi::historization::CSwitch(name, accessMode)))
   {

   }

   //-----------------------------------------------------
   ///\brief                     Destructor
   //-----------------------------------------------------
   CSwitch::~CSwitch()
   {
   }

} //namespace historizers 
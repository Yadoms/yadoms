#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

DECLARE_ENUM_HEADER(ESomfyRemoteControlValues,
((Up))
((Down))
((My))
((Channel))
((Prog))
)

class CRemoteControlValue : public yApi::historization::CSingleHistorizableData<ESomfyRemoteControlValues>
{
public:
   //-----------------------------------------------------
   ///\brief                     Constructor
   ///\param[in] keywordName     Yadoms keyword name
   //-----------------------------------------------------
   explicit CRemoteControlValue(const std::string& keywordName);

   //-----------------------------------------------------
   ///\brief                     Destructor
   //-----------------------------------------------------
   virtual ~CRemoteControlValue();

};




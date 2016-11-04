#include "stdafx.h"
#include "inputOutput.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers
{
   CInputOuput::CInputOuput(const std::string& keywordName, 
                            const std::string& hardwareName,
                            const yApi::EKeywordAccessMode& accessMode)
      :CSingleHistorizableData<bool>(keywordName, yApi::CStandardCapacities::Switch, accessMode, false, yApi::historization::EMeasureType::kAbsolute),
      m_hardwareName(hardwareName)
   {
   }

   std::string CInputOuput::getHardwareName()
   {
      return m_hardwareName;
   }

   CInputOuput::~CInputOuput()
   {}

} // namespace specificHistorizers


#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>


namespace specificHistorizers
{
   DECLARE_ENUM_HEADER(EBlindLockingMode,
      ((DoNotChangeOrNoLock)(0))
      ((Blockage)(1))
      ((Alarm)(2))
      ((Deblockage)(7))
   ) ;

   class CBlindLockingMode : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EBlindLockingMode>
   {
   public:
      explicit CBlindLockingMode(const std::string& keywordName);
      virtual ~CBlindLockingMode();
   };
} // namespace specificHistorizers

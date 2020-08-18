#include "stdafx.h"
#include "VolatileOrganicCompound.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers
{
   DECLARE_ENUM_IMPLEMENTATION(EVolatileOrganicCompound,
                               ((kVOCT__total_))
                               ((kFormaldehyde))
                               ((kBenzene))
                               ((kStyrene))
                               ((kToluene))
                               ((kTetrachloroethylene))
                               ((kXylene))
                               ((kn_Hexane))
                               ((kn_Octane))
                               ((kCyclopentane))
                               ((kMethanol))
                               ((kEthanol))
                               ((k1_Pentanol))
                               ((kAcetone))
                               ((kethylene_Oxide))
                               ((kAcetaldehyde_ue))
                               ((kAcetic_Acid))
                               ((kPropionice_Acid))
                               ((kValeric_Acid))
                               ((kButyric_Acid))
                               ((kAmmoniac))
                               ((kHydrogen_Sulfide))
                               ((kDimethylsulfide))
                               ((k2_Butanol__butyl_Alcohol_))
                               ((k2_Methylpropanol))
                               ((kDiethyl_ether))
                               ((kNaphthalene))
                               ((k4_Phenylcyclohexene))
                               ((kLimonene))
                               ((kTrichloroethylene))
                               ((kIsovaleric_acid))
                               ((kIndole))
                               ((kCadaverine))
                               ((kPutrescine))
                               ((kCaproic_acid))
                               ((kOzone))
   );

   DECLARE_CAPACITY(VolatileOrganicCompoundCapacity,
                    "VolatileOrganicCompound",
                    shared::plugin::yPluginApi::CStandardUnits::NoUnit(),
                    shared::plugin::yPluginApi::EKeywordDataType::kEnum);

   CVolatileOrganicCompound::CVolatileOrganicCompound(const std::string& keywordName)
      : CSingleHistorizableData<EVolatileOrganicCompound>(keywordName,
                                                          VolatileOrganicCompoundCapacity(),
                                                          shared::plugin::yPluginApi::EKeywordAccessMode::kGet)
   {
   }
} // namespace specificHistorizers

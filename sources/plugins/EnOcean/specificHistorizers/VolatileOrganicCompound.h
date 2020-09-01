#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   DECLARE_ENUM_HEADER(EVolatileOrganicCompound,
                       ((kVOCT__total_)(0))
                       ((kFormaldehyde)(1))
                       ((kBenzene)(2))
                       ((kStyrene)(3))
                       ((kToluene)(4))
                       ((kTetrachloroethylene)(5))
                       ((kXylene)(6))
                       ((kn_Hexane)(7))
                       ((kn_Octane)(8))
                       ((kCyclopentane)(9))
                       ((kMethanol)(10))
                       ((kEthanol)(11))
                       ((k1_Pentanol)(12))
                       ((kAcetone)(13))
                       ((kethylene_Oxide)(14))
                       ((kAcetaldehyde_ue)(15))
                       ((kAcetic_Acid)(16))
                       ((kPropionice_Acid)(17))
                       ((kValeric_Acid)(18))
                       ((kButyric_Acid)(19))
                       ((kAmmoniac)(20))
                       ((kHydrogen_Sulfide)(22))
                       ((kDimethylsulfide)(23))
                       ((k2_Butanol__butyl_Alcohol_)(24))
                       ((k2_Methylpropanol)(25))
                       ((kDiethyl_ether)(26))
                       ((kNaphthalene)(27))
                       ((k4_Phenylcyclohexene)(28))
                       ((kLimonene)(29))
                       ((kTrichloroethylene)(30))
                       ((kIsovaleric_acid)(31))
                       ((kIndole)(32))
                       ((kCadaverine)(33))
                       ((kPutrescine)(34))
                       ((kCaproic_acid)(35))
                       ((kOzone)(255))
   ) ;

   class CVolatileOrganicCompound : public yApi::historization::CSingleHistorizableData<
         EVolatileOrganicCompound>
   {
   public:
      explicit CVolatileOrganicCompound(const std::string& keywordName);
      virtual ~CVolatileOrganicCompound() = default;
   };
} // namespace specificHistorizers

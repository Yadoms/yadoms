#pragma once
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace specificHistorizers
{
   //-----------------------------------------------------
   ///\brief A SNR object
   //-----------------------------------------------------
   class CSNR : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<double>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CSNR(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CSNR();
   };
} // namespace specificHistorizers



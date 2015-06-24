#pragma once

#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules { namespace rfxLanXpl { namespace data {

   //-----------------------------------------------------
   ///\brief               The Digimax demand
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(EDigimaxDemand,
      ((HeaterOn))
      ((HeaterOff))
      ((CoolingOn))
      ((CoolingOff))
   );

   //-----------------------------------------------------
   ///\brief A RfxLan digital io historizable object
   //-----------------------------------------------------
   class CDigimaxDemand : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EDigimaxDemand>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CDigimaxDemand(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CDigimaxDemand();

   };



} } } // namespace xplrules::rfxLanXpl::data


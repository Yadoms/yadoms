#pragma once

#include "HistorizableEnum.h"
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules { namespace rfxLanXpl { namespace data {

   //-----------------------------------------------------
   ///\brief               The Digimax demand
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(EDigimaxDemand,
      ((HeaterOn)(0))
      ((HeaterOff)(1))
      ((CoolingOn)(2))
      ((CoolingOff)(3))
   );

   //-----------------------------------------------------
   ///\brief A RfxLan digital io historizable object
   //-----------------------------------------------------
   class CDigimaxDemand : public CHistorizableEnum<EDigimaxDemand>
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


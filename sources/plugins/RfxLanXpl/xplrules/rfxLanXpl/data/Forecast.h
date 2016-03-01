#pragma once

#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules { namespace rfxLanXpl { namespace data {

   //-----------------------------------------------------
   ///\brief               The ninja-robocam command values
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(EForecast,
      ((Sunny))
      ((PartlyCloudy))
      ((Cloudy))
      ((Rain))
   );

   //-----------------------------------------------------
   ///\brief A Ninja historizable object
   //-----------------------------------------------------
   class CForecast : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EForecast>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CForecast(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CForecast();

   };



} } } // namespace xplrules::rfxLanXpl::data


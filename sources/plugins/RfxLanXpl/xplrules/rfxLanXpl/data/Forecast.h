#pragma once

#include "HistorizableEnum.h"
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules { namespace rfxLanXpl { namespace data {

   //-----------------------------------------------------
   ///\brief               The ninja-robocam command values
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(EForecast,
      ((Sunny)(0))
      ((PartlyCloudy)(1))
      ((Cloudy)(2))
      ((Rain)(3))
   );

   //-----------------------------------------------------
   ///\brief A Ninja historizable object
   //-----------------------------------------------------
   class CForecast : public CHistorizableEnum<EForecast>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CForecast(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CForecast();

   };



} } } // namespace xplrules::rfxLanXpl::data


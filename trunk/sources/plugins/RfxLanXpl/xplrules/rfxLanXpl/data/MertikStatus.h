#pragma once

#include "HistorizableEnum.h"
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules { namespace rfxLanXpl { namespace data {

   //-----------------------------------------------------
   ///\brief               The mertik Status values
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(EMertikStatus,
      ((On)(0))
      ((Off)(1))
      ((StepUp)(2))
      ((StepDown)(3))
      ((RunUp)(4))
      ((RunDown)(5))
      ((Stop)(6))
   );

   //-----------------------------------------------------
   ///\brief A mertick historizable object
   //-----------------------------------------------------
   class CMertickStatus : public CHistorizableEnum<EMertikStatus>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CMertickStatus(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CMertickStatus();

   };



} } } // namespace xplrules::rfxLanXpl::data


#pragma once

#include "HistorizableEnum.h"
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules { namespace rfxLanXpl { namespace data {

   //-----------------------------------------------------
   ///\brief               The ninja-robocam command values
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(ENinjaCommand,
      ((On)(0))
      ((Off)(1))
      ((StepUp)(2))
      ((StepDown)(3))
      ((RunUp)(4))
      ((RunDown)(5))
      ((Stop)(6))
   );

   //-----------------------------------------------------
   ///\brief A Ninja historizable object
   //-----------------------------------------------------
   class CNinja : public CHistorizableEnum<ENinjaCommand>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CNinja(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CNinja();

   };



} } } // namespace xplrules::rfxLanXpl::data


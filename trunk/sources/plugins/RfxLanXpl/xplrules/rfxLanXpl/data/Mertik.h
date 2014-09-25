#pragma once

#include "HistorizableEnum.h"
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules { namespace rfxLanXpl { namespace data {

   //-----------------------------------------------------
   ///\brief               The mertik command values
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(EMertikCommand,
      ((Left)(0))
      ((Right)(1))
      ((Up)(2))
      ((Down)(3))
      ((P1)(4))
      ((ProgP1)(5))
      ((P2)(6))
      ((ProgP2)(7))
      ((P3)(8))
      ((ProgP3)(9))
      ((P4)(10))
      ((ProgP4)(11))
      ((Center)(12))
      ((ProgCenter)(13))
      ((Sweep)(14))
      ((ProgSweep)(15))
   );

   //-----------------------------------------------------
   ///\brief A mertick historizable object
   //-----------------------------------------------------
   class CMertick : public CHistorizableEnum<EMertikCommand>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CMertick(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CMertick();

   };



} } } // namespace xplrules::rfxLanXpl::data


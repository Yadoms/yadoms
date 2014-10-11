#pragma once

#include "HistorizableEnum.h"
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules { namespace rfxLanXpl { namespace data {

   //-----------------------------------------------------
   ///\brief               The ninja-robocam command values
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(EUvDescription,
      ((Low)(0))
      ((Medium)(1))
      ((High)(2))
      ((VeryHigh)(3))
      ((Dangerous)(4))
   );

   //-----------------------------------------------------
   ///\brief A Ninja historizable object
   //-----------------------------------------------------
   class CUvDescription : public CHistorizableEnum<EUvDescription>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CUvDescription(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CUvDescription();

   };



} } } // namespace xplrules::rfxLanXpl::data


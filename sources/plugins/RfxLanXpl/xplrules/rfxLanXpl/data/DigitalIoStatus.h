#pragma once

#include "HistorizableEnum.h"
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules { namespace rfxLanXpl { namespace data {

   //-----------------------------------------------------
   ///\brief               The RfxLan digital io status values
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(EDigitalIoStatus,
      ((High)(0))
      ((Low)(1))
   );

   //-----------------------------------------------------
   ///\brief A RfxLan digital io historizable object
   //-----------------------------------------------------
   class CDigitalIoStatus : public CHistorizableEnum<EDigitalIoStatus>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CDigitalIoStatus(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CDigitalIoStatus();

   };



} } } // namespace xplrules::rfxLanXpl::data


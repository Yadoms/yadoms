#pragma once

#include "HistorizableEnum.h"
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules { namespace rfxLanXpl { namespace data {

   //-----------------------------------------------------
   ///\brief               The RfxLan digital io command values
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(EDigitalIoCommand,
      ((High)(0))
      ((Low)(1))
      ((Toggle)(2))
   );

   //-----------------------------------------------------
   ///\brief A RfxLan digital io historizable object
   //-----------------------------------------------------
   class CDigitalIoCommand : public CHistorizableEnum<EDigitalIoCommand>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CDigitalIoCommand(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CDigitalIoCommand();

   };



} } } // namespace xplrules::rfxLanXpl::data


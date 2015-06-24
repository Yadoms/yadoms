#pragma once
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace teleInfoUSB { namespace specificHistorizers
{   
   //-----------------------------------------------------
   ///\brief  The color of the next period
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(EColor,
      ((BLUE))
      ((WHITE))
	  ((RED))
   );

   //-----------------------------------------------------
   ///\brief A Color object
   //-----------------------------------------------------
   class CColor : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EColor>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      The access mode
      ///\param[in] measureType     The measure type (normally kAbsolute)
      //-----------------------------------------------------
      CColor(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CColor();
   };

} } // namespace teleInfoUSB::specificHistorizer


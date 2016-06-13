#pragma once
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

//TODO V2 : To be copy in a library teleInfo

namespace teleInfo
{   
   //-----------------------------------------------------
   ///\brief  The color of the next period
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(EColor,
      ((NOTDEFINED))
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
      explicit CColor(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CColor();
   };

} // namespace teleInfo


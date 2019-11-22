#pragma once
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace specificHistorizers
{
   //-----------------------------------------------------
   ///\brief  Status of the TeleInfo
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(EWESTeleInfoStatus,
      ((Ok))
      ((Error))
      ((TeleInfoLost))
      ((Desactivated))
      );

   //-----------------------------------------------------
   ///\brief A Status object
   //-----------------------------------------------------
   class CTeleInfoStatus : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EWESTeleInfoStatus>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CTeleInfoStatus(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CTeleInfoStatus();
   };
} // namespace specificHistorizers



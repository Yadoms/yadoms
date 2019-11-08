#pragma once
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

//TODO_V2 : To be copy in a library teleInfo
namespace teleInfo
{
   namespace specificHistorizers
   {
      //-----------------------------------------------------
      ///\brief  Status of the TeleInfo
      //-----------------------------------------------------   
      DECLARE_ENUM_HEADER(EStatus,
         ((Ok))
         ((Error))
         ((Desactivated))
         );

      //-----------------------------------------------------
      ///\brief A Status object
      //-----------------------------------------------------
      class CTeleInfoStatus : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EStatus>
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
   }
} // namespace teleInfo::specificHistorizer



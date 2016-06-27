#pragma once

#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules
{
   namespace rfxLanXpl
   {
      namespace data
      {
         //-----------------------------------------------------
         ///\brief               The RfxLan digital io status values
         //-----------------------------------------------------   
         DECLARE_ENUM_HEADER(EDigitalIoStatus,
            ((High))
            ((Low))
         ) ;

         //-----------------------------------------------------
         ///\brief A RfxLan digital io historizable object
         //-----------------------------------------------------
         class CDigitalIoStatus : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EDigitalIoStatus>
         {
         public:
            //-----------------------------------------------------
            ///\brief                     Constructor
            ///\param[in] keywordName     Yadoms keyword name
            //-----------------------------------------------------
            explicit CDigitalIoStatus(const std::string& keywordName);

            //-----------------------------------------------------
            ///\brief                     Destructor
            //-----------------------------------------------------
            virtual ~CDigitalIoStatus();
         };
      }
   }
} // namespace xplrules::rfxLanXpl::data



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
         ///\brief               The RfxLan digital io command values
         //-----------------------------------------------------   
         DECLARE_ENUM_HEADER(EDigitalIoCommand,
            ((High))
            ((Low))
            ((Toggle))
         ) ;

         //-----------------------------------------------------
         ///\brief A RfxLan digital io historizable object
         //-----------------------------------------------------
         class CDigitalIoCommand : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EDigitalIoCommand>
         {
         public:
            //-----------------------------------------------------
            ///\brief                     Constructor
            ///\param[in] keywordName     Yadoms keyword name
            //-----------------------------------------------------
            explicit CDigitalIoCommand(const std::string& keywordName);

            //-----------------------------------------------------
            ///\brief                     Destructor
            //-----------------------------------------------------
            virtual ~CDigitalIoCommand();
         };
      }
   }
} // namespace xplrules::rfxLanXpl::data



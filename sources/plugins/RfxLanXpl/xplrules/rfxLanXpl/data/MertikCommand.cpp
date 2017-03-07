#include "stdafx.h"
#include "MertikCommand.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace xplrules
{
   namespace rfxLanXpl
   {
      namespace data
      {
         DECLARE_ENUM_IMPLEMENTATION(EMertikCommand,
            ((Left)("left"))
            ((Right)("right"))
            ((Up)("up"))
            ((Down)("down"))
            ((P1)("p1"))
            ((ProgP1)("prog_p1"))
            ((P2)("p2"))
            ((ProgP2)("prog_p2"))
            ((P3)("p3"))
            ((ProgP3)("prog_p3"))
            ((P4)("p4"))
            ((ProgP4)("prog_p4"))
            ((Center)("center"))
            ((ProgCenter)("prog_center"))
            ((Sweep)("sweep"))
            ((ProgSweep)("prog_sweep"))
         );

         DECLARE_CAPACITY(MertikCommandCapacity, "mertik", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kNoData);


         CMertickCommand::CMertickCommand(const std::string& keywordName)
            : CSingleHistorizableData<EMertikCommand>(keywordName,
                                                      MertikCommandCapacity(),
                                                      shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
         {
         }

         CMertickCommand::~CMertickCommand()
         {
         }
      }
   }
} // namespace xplrules::rfxLanXpl::data



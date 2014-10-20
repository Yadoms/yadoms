#include "stdafx.h"
#include "Mertik.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

DECLARE_ENUM_IMPLEMENTATION_CUSTOM(EMertikCommand,
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

const shared::plugin::yadomsApi::CStandardCapacity& MertikCapacity = shared::plugin::yadomsApi::CStandardCapacity("mertik", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::kGetSet, shared::plugin::yadomsApi::kNoData);

   
CMertick::CMertick(const std::string& keywordName)
   :CHistorizableEnum<EMertikCommand>(keywordName, MertikCapacity, "command")
{
}

CMertick::~CMertick()
{
}




















} } } // namespace xplrules::rfxLanXpl::data


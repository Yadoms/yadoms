#pragma once

#include <shared/enumeration/EnumHelpers.hpp>

//-----------------------------------------------------
///\brief   Internal ZWave plugin state
//-----------------------------------------------------
DECLARE_ENUM_HEADER(EZWaveInteralState, 
   ((Running))
   ((DriverReady))
   ((DriverFailed))
   ((Waiting))
   ((Completed))
)
#include "stdafx.h"
#include "ZWaveInternalState.h"


DECLARE_ENUM_IMPLEMENTATION(EZWaveInteralState,
   ((Running))
   ((DriverReady))
   ((DriverFailed))
   ((Waiting))
   ((WaitingInclusion))
   ((WaitingExclusion))
   ((Completed))
)
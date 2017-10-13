#include "stdafx.h"
#include "ZWaveInternalState.h"


DECLARE_ENUM_IMPLEMENTATION(EZWaveInteralState,
   ((Running)("running"))
   ((DriverReady)("driverReady"))
   ((DriverFailed)("driverFailed"))
   ((Waiting)("waiting"))
   ((WaitingInclusion)("waitingInclusion"))
   ((WaitingExclusion)("waitingExclusion"))
   ((Completed)("completed"))
)
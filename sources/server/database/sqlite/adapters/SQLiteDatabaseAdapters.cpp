#include "stdafx.h"
#include "SQLiteDatabaseAdapters.h"
#include "sqlite3.h"
#include <shared/Log.h>
#include "AdapterHelpers.hpp"


DECLARE_ADAPTER_IMPLEMENTATION(Hardware,
   ((Id)(int)(0))
   ((Name)(std::string)(""))
   ((PluginName)(std::string)(""))
   ((Configuration)(std::string)(""))
   ((Enabled)(bool)(true))
   ((Deleted)(bool)(false))
   )

DECLARE_ADAPTER_IMPLEMENTATION(Configuration,
   ((Section)(std::string)(""))
   ((Name)(std::string)(""))
   ((Value)(std::string)(""))
   ((DefaultValue)(std::string)(""))
   ((Description)(std::string)(""))
   ((SecurityAccess)(CConfiguration::ESecurityAccess)(CConfiguration::kNone)(int))
   ((LastModificationDate)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
   )

DECLARE_ADAPTER_IMPLEMENTATION(Acquisition,
   ((Id)(int)(0))
   ((Source)(std::string)(""))
   ((Keyword)(std::string)(""))
   ((Value)(std::string)(""))
   ((Date)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
   )

DECLARE_ADAPTER_IMPLEMENTATION(Device,
   ((Id)(int)(0))
   ((DataSource)(std::string)(""))
   ((Name)(std::string)(""))
   ((Configuration)(std::string)(""))
   )

DECLARE_ADAPTER_IMPLEMENTATION(Keyword,
   ((Name)(std::string)(""))
   )

DECLARE_ADAPTER_IMPLEMENTATION(Page,
   ((Id)(int)(0))
   ((Name)(std::string)(""))
   ((PageOrder)(int)(0))
   )


DECLARE_ADAPTER_IMPLEMENTATION(Widget,
   ((Id)(int)(0))
   ((IdPage)(int)(0))
   ((Name)(std::string)(""))
   ((SizeX)(int)(1))
   ((SizeY)(int)(1))
   ((PositionX)(int)(1))
   ((PositionY)(int)(1))
   ((Configuration)(std::string)(""))
   )

DECLARE_ADAPTER_IMPLEMENTATION(HardwareEventLogger,
   ((Id)(int)(0))
   ((EventDate)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
   ((PluginName)(std::string)(""))
   ((PluginVersion)(std::string)(""))
   ((PluginRelease)(shared::plugin::information::EReleaseType)(shared::plugin::information::kStable)(int))
   ((EventType)(CHardwareEventLogger::EEventType)(CHardwareEventLogger::kUnload)(int))
   ((Message)(std::string)(""))
)

DECLARE_ADAPTER_IMPLEMENTATION(EventLogger,
   ((Id)(int)(0))
   ((EventDate)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
   ((EventType)(std::string)(""))
   ((OptionalData)(std::string)(""))
)



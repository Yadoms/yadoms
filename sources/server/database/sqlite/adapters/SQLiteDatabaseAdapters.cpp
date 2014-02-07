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
   ((LastModificationDate)(boost::posix_time::ptime)(boost::posix_time::second_clock::local_time()))
   )

DECLARE_ADAPTER_IMPLEMENTATION(Acquisition,
   ((Id)(int)(0))
   ((Source)(std::string)(""))
   ((Keyword)(std::string)(""))
   ((Value)(std::string)(""))
   ((Date)(boost::posix_time::ptime)(boost::posix_time::second_clock::local_time()))
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
   )


DECLARE_ADAPTER_IMPLEMENTATION(Widget,
   ((Id)(int)(0))
   ((IdPage)(int)(0))
   ((Name)(std::string)(""))
   ((SizeX)(int)(0))
   ((SizeY)(int)(0))
   ((PositionX)(int)(0))
   ((PositionY)(int)(0))
   ((Configuration)(std::string)(""))
   )

DECLARE_ADAPTER_IMPLEMENTATION(HardwareEventLogger,
   ((Id)(int)(0))
   ((EventDate)(boost::posix_time::ptime)(boost::posix_time::second_clock::local_time()))
   ((PluginName)(std::string)(""))
   ((PluginVersion)(std::string)(""))
   ((PluginRelease)(IHardwarePluginInformation::EReleaseType)(IHardwarePluginInformation::EReleaseType::kStable))
   ((EventType)(CHardwareEventLogger::EEventType)(CHardwareEventLogger::EEventType::kUnload))
   ((Message)(std::string)(""))
)   
   
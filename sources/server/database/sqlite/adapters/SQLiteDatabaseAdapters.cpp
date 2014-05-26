#include "stdafx.h"
#include "SQLiteDatabaseAdapters.h"
#include "sqlite3.h"
#include <shared/Log.h>
#include "AdapterHelpers.hpp"


namespace database { 
namespace sqlite { 
namespace adapters { 


   DECLARE_ADAPTER_IMPLEMENTATION(Plugin,
      ((Id)(int)(0))
      ((Name)(std::string)(""))
      ((Type)(std::string)(""))
      ((Configuration)(std::string)(""))
      ((AutoStart)(bool)(true))
      )

   DECLARE_ADAPTER_IMPLEMENTATION(Configuration,
      ((Section)(std::string)(""))
      ((Name)(std::string)(""))
      ((Value)(std::string)(""))
      ((DefaultValue)(std::string)(""))
      ((Description)(std::string)(""))
      ((SecurityAccess)(database::entities::ESecurityAccess)(database::entities::kNone)(int))
      ((LastModificationDate)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
      )


   DECLARE_ADAPTER_IMPLEMENTATION(Keyword,
      ((DeviceId)(int)(0))
      ((Name)(std::string)(""))
      ((Units)(std::string)(""))
      ((Type)(std::string)(""))
      ((Minimum)(double)(0))
      ((Maximum)(double)(0))
      ((Parameters)(std::string)(""))
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

   DECLARE_ADAPTER_IMPLEMENTATION(PluginEventLogger,
      ((Id)(int)(0))
      ((EventDate)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
      ((PluginName)(std::string)(""))
      ((PluginVersion)(std::string)(""))
      ((PluginRelease)(shared::plugin::information::EReleaseType)(shared::plugin::information::kStable)(int))
      ((EventType)(database::entities::EEventType)(database::entities::kUnload)(int))
      ((Message)(std::string)(""))
   )

   DECLARE_ADAPTER_IMPLEMENTATION(EventLogger,
      ((Id)(int)(0))
      ((Date)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
      ((Code)(database::entities::ESystemEventCode)(database::entities::kDefaultCode)(int))
      ((Who)(std::string)(""))
      ((What)(std::string)(""))
   )


   DECLARE_ADAPTER_IMPLEMENTATION(Message,
      ((Id)(int)(0))
      ((Date)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
      ((DeviceId)(int)(0))
   )

   DECLARE_ADAPTER_IMPLEMENTATION(MessageContent,
      ((Id)(int)(0))
      ((IdMessage)(int)(0))
      ((Key)(std::string)(""))
      ((Value)(std::string)(""))
   )

   DECLARE_ADAPTER_IMPLEMENTATION(Device,
      ((Id)(int)(0))
      ((Address)(std::string)(""))
      ((Protocol)(std::string)(""))
      ((HardwareIdentifier)(std::string)(""))
      ((Name)(std::string)(""))
      )



} //namespace adapters
} //namespace sqlite
} //namespace database 


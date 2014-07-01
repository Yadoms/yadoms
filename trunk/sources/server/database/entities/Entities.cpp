#include "stdafx.h"
#include "Entities.h"

namespace database { namespace entities {


   DECLARE_ENTITY_CLASS_IMPLEMENTATION(Plugin,
      ((Id)(int)(0))
      ((Name)(std::string)(""))
      ((Type)(std::string)(""))
      ((Configuration)(shared::CDataContainer)())
      ((AutoStart)(bool)(true))
      )


   DECLARE_ENTITY_CLASS_IMPLEMENTATION(Configuration,
      ((Section)(std::string)(""))
      ((Name)(std::string)(""))
      ((Value)(std::string)(""))
      ((DefaultValue)(std::string)(""))
      ((Description)(std::string)(""))
      ((SecurityAccess)(database::entities::ESecurityAccess)(database::entities::kNone))
      ((LastModificationDate)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
      )

   DECLARE_ENTITY_CLASS_IMPLEMENTATION(Page,
      ((Id)(int)(0))
      ((Name)(std::string)(""))
      ((PageOrder)(int)(0))
      )


   DECLARE_ENTITY_CLASS_IMPLEMENTATION(Widget,
      ((Id)(int)(0))
      ((IdPage)(int)(0))
      ((Name)(std::string)(""))
      ((SizeX)(int)(1))
      ((SizeY)(int)(1))
      ((PositionX)(int)(1))
      ((PositionY)(int)(1))
      ((Configuration)(shared::CDataContainer)(shared::CDataContainer()))
      )

   DECLARE_ENTITY_CLASS_IMPLEMENTATION(PluginEventLogger,
      ((Id)(int)(0))
      ((EventDate)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
      ((PluginName)(std::string)(""))
      ((PluginVersion)(std::string)(""))
      ((PluginRelease)(shared::plugin::information::EReleaseType)(shared::plugin::information::kStable))
      ((EventType)(database::entities::EEventType)(database::entities::kUnload))
      ((Message)(std::string)(""))
   )

   DECLARE_ENTITY_CLASS_IMPLEMENTATION(EventLogger,
      ((Id)(int)(0))
      ((Date)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
      ((Code)(database::entities::ESystemEventCode)(database::entities::kDefaultCode))
      ((Who)(std::string)(""))
      ((What)(std::string)(""))
   )


   DECLARE_ENTITY_CLASS_IMPLEMENTATION(Device,
      ((Id)(int)(0))
      ((PluginId)(int)(0))
      ((Name)(std::string)(""))
      ((FriendlyName)(std::string)(""))
      ((Model)(std::string)(""))
	  ((Details)(std::string)(""))
	  )

   DECLARE_ENTITY_CLASS_IMPLEMENTATION(Keyword,
      ((Id)(int)(0))
      ((DeviceId)(int)(0))
      ((CapacityName)(std::string)(""))
      ((AccessMode)(database::entities::EKeywordAccessMode)(database::entities::kNoAccess))
      ((Name)(std::string)(""))
      ((FriendlyName)(std::string)(""))
      ((Type)(database::entities::EKeywordDataType)(database::entities::kString))
      ((Units)(std::string)(""))
      ((Details)(shared::CDataContainer)(shared::CDataContainer()))
   )

   DECLARE_ENTITY_CLASS_IMPLEMENTATION(Acquisition,
      ((Id)(int)(0))
      ((Date)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
      ((KeywordId)(int)(0))
      ((Value)(std::string)(""))
   )



} //namespace entities
} //namespace database

#include "stdafx.h"
#include "Entities.h"

namespace database { namespace entities {
   
   DECLARE_ENUM_IMPLEMENTATION(ESecurityAccess,
      (None)
      (Admin)
      (User)
   )

   DECLARE_ENUM_IMPLEMENTATION(EEventType,
      (Unload)
      (Load)
      (Crash)
      (Info)
      (Error)
   )

   DECLARE_ENUM_IMPLEMENTATION(ESystemEventCode,
      (TaskFailed)
      (PluginDisabled)
      (PluginCrash)
      (YadomsCrash)
      (DefaultCode)
      (Started)
      (Stopped)
      (Updated)
   )

   DECLARE_ENUM_IMPLEMENTATION(EKeywordDataType,
      (NoData)
      (String)
      (Numeric)
      (Bool)
      (Json)
   )

   DECLARE_ENUM_IMPLEMENTATION(EKeywordAccessMode,
      (NoAccess)
      (Get)
      (Set)
      (GetSet)
   )


   DECLARE_ENTITY_CLASS_IMPLEMENTATION(
   Plugin,
      ((Id)(int)(0)("id"))
      ((Name)(std::string)("")("name"))
      ((Type)(std::string)("")("type"))
      ((Configuration)(shared::CDataContainer)(shared::CDataContainer())("configuration"))
      ((AutoStart)(bool)(true)("autoStart"))
      );


   DECLARE_ENTITY_CLASS_IMPLEMENTATION(
   Configuration,
      ((Section)(std::string)("")("section"))
      ((Name)(std::string)("")("name"))
      ((Value)(std::string)("")("value"))
      ((DefaultValue)(std::string)("")("defaultValue"))
      ((Description)(std::string)("")("description"))
      ((SecurityAccess)(database::entities::ESecurityAccess)(database::entities::ESecurityAccess::kNone)("securityAccess"))
      ((LastModificationDate)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time())("lastModificationDate"))
      );

   DECLARE_ENTITY_CLASS_IMPLEMENTATION(
   Page,
      ((Id)(int)(0)("id"))
      ((Name)(std::string)("")("name"))
      ((PageOrder)(int)(0)("pageOrder"))
      );


   DECLARE_ENTITY_CLASS_IMPLEMENTATION(
   Widget,
      ((Id)(int)(0)("id"))
      ((IdPage)(int)(0)("idPage"))
      ((Name)(std::string)("")("name"))
      ((SizeX)(int)(1)("sizeX"))
      ((SizeY)(int)(1)("sizeY"))
      ((PositionX)(int)(1)("positionX"))
      ((PositionY)(int)(1)("positionY"))
      ((Configuration)(shared::CDataContainer)(shared::CDataContainer())("configuration"))
      );

   DECLARE_ENTITY_CLASS_IMPLEMENTATION(PluginEventLogger,
   ((Id)(int)(0)("id"))
      ((EventDate)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time())("eventDate"))
      ((PluginName)(std::string)("")("pluginName"))
      ((PluginVersion)(std::string)("")("pluginVersion"))
      ((PluginRelease)(shared::plugin::information::EReleaseType)(shared::plugin::information::kStable)("pluginRelease"))
      ((EventType)(database::entities::EEventType)(database::entities::EEventType::kUnload)("eventType"))
      ((Message)(std::string)("")("message"))
      );

   DECLARE_ENTITY_CLASS_IMPLEMENTATION(
   EventLogger,
      ((Id)(int)(0)("id"))
      ((Date)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time())("date"))
      ((Code)(database::entities::ESystemEventCode)(database::entities::ESystemEventCode::kDefaultCode)("code"))
      ((Who)(std::string)("")("who"))
      ((What)(std::string)("")("what"))
      );


   DECLARE_ENTITY_CLASS_IMPLEMENTATION(
   Device,
      ((Id)(int)(0)("id"))
      ((PluginId)(int)(0)("pluginId"))
      ((Name)(std::string)("")("name"))
      ((FriendlyName)(std::string)("")("friendlyName"))
      ((Model)(std::string)("")("model"))
      ((Details)(shared::CDataContainer)(shared::CDataContainer())("details"))
      );

   DECLARE_ENTITY_CLASS_IMPLEMENTATION(
   Keyword,
      ((Id)(int)(0)("id"))
      ((DeviceId)(int)(0)("deviceId"))
      ((CapacityName)(std::string)("")("capacityName"))
      ((AccessMode)(database::entities::EKeywordAccessMode)(database::entities::EKeywordAccessMode::kNoAccess)("accessMode"))
      ((Name)(std::string)("")("name"))
      ((FriendlyName)(std::string)("")("friendlyName"))
      ((Type)(database::entities::EKeywordDataType)(database::entities::EKeywordDataType::kString)("type"))
      ((Units)(std::string)("")("units"))
      ((Details)(shared::CDataContainer)(shared::CDataContainer())("details"))
      );

   DECLARE_ENTITY_CLASS_IMPLEMENTATION(
   Acquisition,
      ((Id)(int)(0)("id"))
      ((Date)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time())("date"))
      ((KeywordId)(int)(0)("keywordId"))
      ((Value)(std::string)("")("value"))
      );



} //namespace entities
} //namespace database

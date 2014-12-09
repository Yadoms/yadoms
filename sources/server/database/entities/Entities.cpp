#include "stdafx.h"
#include "Entities.h"

namespace database { namespace entities {
   
   DECLARE_ENUM_IMPLEMENTATION(ESecurityAccess,
      ((None))
      ((Admin))
      ((User))
   )

   DECLARE_ENUM_IMPLEMENTATION(EEventType,
      ((Unload))
      ((Load))
      ((Crash))
      ((Info))
      ((Error))
   )

   DECLARE_ENUM_IMPLEMENTATION(EPluginCategory,
      ((System))
      ((User))
   )
   
   DECLARE_ENUM_IMPLEMENTATION(EAcquisitionSummaryType,
      ((Hour))
      ((Day))
   )


   DECLARE_ENUM_IMPLEMENTATION(ESystemEventCode,
      ((ThreadFailed))
      ((TaskFailed))
      ((PluginDisabled))
      ((PluginCrash))
      ((YadomsCrash))
      ((DefaultCode))
      ((Started))
      ((Stopped))
      ((Updated))
   )



   DECLARE_ENTITY_CLASS_IMPLEMENTATION(
   Plugin,
      ((Id)(int)(0)("id"))
      ((Name)(std::string)("")("name"))
      ((Type)(std::string)("")("type"))
      ((Configuration)(shared::CDataContainer)(shared::CDataContainer())("configuration"))
      ((AutoStart)(bool)(true)("autoStart"))
      ((Category)(database::entities::EPluginCategory)(database::entities::EPluginCategory::kSystem)("category"))
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

   DECLARE_ENTITY_CLASS_IMPLEMENTATION(
   PluginEventLogger,
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
      ((AccessMode)(shared::plugin::yPluginApi::EKeywordAccessMode)(shared::plugin::yPluginApi::EKeywordAccessMode::kNoAccess)("accessMode"))
      ((Name)(std::string)("")("name"))
      ((FriendlyName)(std::string)("")("friendlyName"))
      ((Type)(shared::plugin::yPluginApi::EKeywordDataType)(shared::plugin::yPluginApi::EKeywordDataType::kString)("type"))
      ((Units)(std::string)("")("units"))
      ((Measure)(shared::plugin::yPluginApi::historization::EMeasureType)(shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute)("measure"))
      ((Details)(shared::CDataContainer)(shared::CDataContainer())("details"))
      );

   DECLARE_ENTITY_CLASS_IMPLEMENTATION(
      Acquisition,
      ((Date)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time())("date"))
      ((KeywordId)(int)(0)("keywordId"))
      ((Value)(std::string)("")("value"))
      );

     
   DECLARE_ENTITY_CLASS_IMPLEMENTATION(
      AcquisitionSummary,
      ((Type)(database::entities::EAcquisitionSummaryType)(database::entities::EAcquisitionSummaryType::kHour)("type"))
      ((Date)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time())("date"))
      ((KeywordId)(int)(0)("keywordId"))
      ((Avg)(std::string)("")("avg"))
      ((Min)(std::string)("")("min"))
      ((Max)(std::string)("")("max"))
   );


   DECLARE_ENTITY_CLASS_IMPLEMENTATION(
      Job,
      ((Id)(int)(0)("id"))
      ((Name)(std::string)("")("name"))
      ((Description)(std::string)("")("description"))
      ((Triggers)(shared::CDataContainer)(shared::CDataContainer())("triggers"))
      ((Actions)(shared::CDataContainer)(shared::CDataContainer())("actions"))
      ((Enable)(bool)(true)("enable"))
   );


//   DECLARE_ENTITY_CLASS_IMPLEMENTATION(
//      Contact,
//      ((Id)(int)(0)("id"))
//      ((Identity)(shared::CDataContainer)(shared::CDataContainer())("identity"))
//      ((Information)(shared::CDataContainer)(shared::CDataContainer())("information"))
//   );



} //namespace entities
} //namespace database

#pragma once
#include "EntityHelpers.hpp"
#include <shared/plugin/information/IInformation.h>
#include <shared/DataContainer.h>
#include <shared/IDataContainable.h>
#include "Field.hpp"
#include <shared/enumeration/IExtendedEnum.h>
#include "EnumHelpers.hpp"

namespace database { namespace entities {

   DECLARE_ENUM_HEADER(SecurityAccess,
      ((None)(0))
      ((Admin)(1))
      ((User)(2))
   )

   DECLARE_ENUM_HEADER(EventType,
      ((Unload)(0))
      ((Load)(1))
      ((Crash)(2))
      ((Info)(3))
      ((Error)(4))
   )

   DECLARE_ENUM_HEADER(SystemEventCode,
      ((TaskFailed)(-4))
      ((PluginDisabled)(-3))
      ((PluginCrash)(-2))
      ((YadomsCrash)(-1))
      ((DefaultCode)(0))
      ((Started)(1))
      ((Stopped)(2))
      ((Updated)(3))
   )

   DECLARE_ENUM_HEADER(KeywordDataType,
      ((NoData)(0)) 
      ((String)(1))
      ((Numeric)(2))
      ((Bool)(3))
      ((Json)(4))
   )
   
   DECLARE_ENUM_HEADER(KeywordAccessMode,
      ((NoAccess)(0))
      ((Get)(1))
      ((Set)(2))
      ((GetSet)(3))
   )

   DECLARE_ENTITY_CLASS_HEADER(Plugin,
      ((Id)(int))
      ((Name)(std::string))
      ((Type)(std::string))
      ((Configuration)(shared::CDataContainer))
      ((AutoStart)(bool))
   )

   DECLARE_ENTITY_CLASS_HEADER(Configuration,
      ((Section)(std::string))
      ((Name)(std::string))
      ((Value)(std::string))
      ((DefaultValue)(std::string))
      ((Description)(std::string))
      ((SecurityAccess)(database::entities::ESecurityAccess))
      ((LastModificationDate)(boost::posix_time::ptime))
   )



   DECLARE_ENTITY_CLASS_HEADER(Page,
      ((Id)(int))
      ((Name)(std::string))
      ((PageOrder)(int))
   )


   DECLARE_ENTITY_CLASS_HEADER(Widget,
      ((Id)(int))
      ((IdPage)(int))
      ((Name)(std::string))
      ((SizeX)(int))
      ((SizeY)(int))
      ((PositionX)(int))
      ((PositionY)(int))
      ((Configuration)(shared::CDataContainer))
   )

   DECLARE_ENTITY_CLASS_HEADER(PluginEventLogger,
      ((Id)(int))
      ((EventDate)(boost::posix_time::ptime))
      ((PluginName)(std::string))
      ((PluginVersion)(std::string))
      ((PluginRelease)(shared::plugin::information::EReleaseType))
      ((EventType)(database::entities::EEventType))
      ((Message)(std::string))
   )

   DECLARE_ENTITY_CLASS_HEADER(EventLogger,
      ((Id)(int))
      ((Date)(boost::posix_time::ptime))
      ((Code)(database::entities::ESystemEventCode))
      ((Who)(std::string))
      ((What)(std::string))
   )



   DECLARE_ENTITY_CLASS_HEADER(Device,
      ((Id)(int))
      ((PluginId)(int))
      ((Name)(std::string))
      ((FriendlyName)(std::string))
      ((Model)(std::string))
      ((Details)(shared::CDataContainer))
   )

   DECLARE_ENTITY_CLASS_HEADER(Keyword,
      ((Id)(int))
      ((DeviceId)(int))
      ((CapacityName)(std::string))
      ((AccessMode)(database::entities::EKeywordAccessMode))
      ((Name)(std::string))
      ((FriendlyName)(std::string))
      ((Type)(database::entities::EKeywordDataType))
      ((Units)(std::string))
      ((Details)(shared::CDataContainer))
   )

   DECLARE_ENTITY_CLASS_HEADER(Acquisition,
      ((Id)(int))
      ((Date)(boost::posix_time::ptime))
      ((KeywordId)(int))
      ((Value)(std::string))
   )



} //namespace entities
} //namespace database

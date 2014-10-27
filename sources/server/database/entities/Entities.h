#pragma once
#include "EntityHelpers.hpp"
#include <shared/plugin/information/IInformation.h>
#include <shared/DataContainer.h>
#include <shared/IDataContainable.h>
#include <shared/Field.hpp>
#include <shared/enumeration/IExtendedEnum.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yadomsApi/historization/IHistorizable.h>

namespace database { namespace entities {
   DECLARE_ENUM_HEADER(ESecurityAccess,
      ((None))
      ((Admin))
      ((User))
   )

   DECLARE_ENUM_HEADER(EEventType,
      ((Unload))
      ((Load))
      ((Crash))
      ((Info))
      ((Error))
   )   
   
   DECLARE_ENUM_HEADER(EAcquisitionSummaryType,
      ((Hour))
      ((Day))
   )

   DECLARE_ENUM_HEADER(ESystemEventCode,
      ((ThreadFailed)(-5))
      ((TaskFailed)(-4))
      ((PluginDisabled)(-3))
      ((PluginCrash)(-2))
      ((YadomsCrash)(-1))
      ((DefaultCode)(0))
      ((Started)(1))
      ((Stopped)(2))
      ((Updated)(3))
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
      ((AccessMode)(shared::plugin::yadomsApi::EKeywordAccessMode))
      ((Name)(std::string))
      ((FriendlyName)(std::string))
      ((Type)(shared::plugin::yadomsApi::EKeywordDataType))
      ((Units)(std::string))
      ((Measure)(shared::plugin::yadomsApi::historization::EMeasureType))
      ((Details)(shared::CDataContainer))
   )

   DECLARE_ENTITY_CLASS_HEADER(Acquisition,
      ((Date)(boost::posix_time::ptime))
      ((KeywordId)(int))
      ((Value)(std::string))
   )


   DECLARE_ENTITY_CLASS_HEADER(AcquisitionSummary,
      ((Type)(database::entities::EAcquisitionSummaryType))
      ((Date)(boost::posix_time::ptime))
      ((KeywordId)(int))
      ((Avg)(std::string))
      ((Min)(std::string))
      ((Max)(std::string))
   )








} //namespace entities
} //namespace database

#pragma once
#include "EntityHelpers.hpp"
#include <shared/plugin/information/IInformation.h>

namespace database { 
namespace entities {

   enum ESecurityAccess
   {
      kNone = 0, 
      kAdmin = 1, 
      kUser = 2  
   };

   enum EEventType
   {
      kUnload = 0, 
      kLoad = 1, 
      kCrash = 2  
   };
   
   DECLARE_ENTITY_CLASS(Plugin,
      ((Id)(int)(0))
      ((Name)(std::string)(""))
      ((PluginName)(std::string)(""))
      ((Configuration)(std::string)(""))
      ((Enabled)(bool)(true))
      )

   DECLARE_ENTITY_CLASS(Configuration,
      ((Section)(std::string)(""))
      ((Name)(std::string)(""))
      ((Value)(std::string)(""))
      ((DefaultValue)(std::string)(""))
      ((Description)(std::string)(""))
      ((SecurityAccess)(database::entities::ESecurityAccess)(database::entities::kNone))
      ((LastModificationDate)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
      )

   DECLARE_ENTITY_CLASS(Keyword,
      ((Name)(std::string)(""))
      )

   DECLARE_ENTITY_CLASS(Page,
      ((Id)(int)(0))
      ((Name)(std::string)(""))
      ((PageOrder)(int)(0))
      )


   DECLARE_ENTITY_CLASS(Widget,
      ((Id)(int)(0))
      ((IdPage)(int)(0))
      ((Name)(std::string)(""))
      ((SizeX)(int)(1))
      ((SizeY)(int)(1))
      ((PositionX)(int)(1))
      ((PositionY)(int)(1))
      ((Configuration)(std::string)(""))
      )

   DECLARE_ENTITY_CLASS(PluginEventLogger,
      ((Id)(int)(0))
      ((EventDate)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
      ((PluginName)(std::string)(""))
      ((PluginVersion)(std::string)(""))
      ((PluginRelease)(shared::plugin::information::EReleaseType)(shared::plugin::information::kStable))
      ((EventType)(database::entities::EEventType)(database::entities::kUnload))
      ((Message)(std::string)(""))
   )

   DECLARE_ENTITY_CLASS(EventLogger,
      ((Id)(int)(0))
      ((Date)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
      ((Code)(std::string)(""))
      ((OptionalData)(std::string)(""))
   )

   DECLARE_ENTITY_CLASS(Message,
      ((Id)(int)(0))
      ((Date)(boost::posix_time::ptime)(boost::posix_time::second_clock::universal_time()))
      ((DeviceId)(int)(0))
   )

   DECLARE_ENTITY_CLASS(MessageContent,
      ((Id)(int)(0))
      ((IdMessage)(int)(0))
      ((Key)(std::string)(""))
      ((Value)(std::string)(""))
   )


   DECLARE_ENTITY_CLASS(Device,
      ((Id)(int)(0))
      ((Address)(std::string)(""))
      ((Protocol)(std::string)(""))
      ((HardwareIdentifier)(std::string)(""))
      ((Name)(std::string)(""))
      )

} //namespace entities
} //namespace database

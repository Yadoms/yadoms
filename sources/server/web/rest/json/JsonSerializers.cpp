#include "stdafx.h"

#include "JsonSerializers.h"
#include "JsonSerializersHelpers.hpp"
#include <shared/plugin/information/IInformation.h>
#include <shared/serialization/PTreeToJsonSerializer.h>
#include <shared/exception/InvalidParameter.hpp>
#include "database/entities/Entities.h"

namespace web { namespace rest { namespace json {

   DECLARE_JSON_ENTITY_IMPLEMENTATION(Plugin, "Plugin",
   ((Id)("id")JSON_INT)
      ((Name)("name")JSON_STRING)
      ((PluginName)("pluginName")JSON_STRING)
      ((Configuration)("configuration")JSON_STRING)
      ((Enabled)("enabled")JSON_BOOL)
      )



      DECLARE_JSON_ENTITY_IMPLEMENTATION(Configuration,  "Configuration",
   ((Section)("section")JSON_STRING)
      ((Name)("name")JSON_STRING)
      ((Value)("value")JSON_STRING)
      ((DefaultValue)("default_value")JSON_STRING)
      ((Description)("description")JSON_STRING)
      ((SecurityAccess)("securityAccess")JSON_ENUM(database::entities::ESecurityAccess))
      ((LastModificationDate)("last_modification_date")JSON_DATE)
      )

      DECLARE_JSON_ENTITY_IMPLEMENTATION(Device,  "Device",
   ((Id)("id")JSON_INT)
      ((Address)("address")JSON_STRING)
      ((Protocol)("protocol")JSON_STRING)
      ((Name)("name")JSON_STRING)
      ((HardwareIdentifier)("hardwareIdentifier")JSON_STRING)
      )


      DECLARE_JSON_ENTITY_IMPLEMENTATION(Keyword,  "Keyword",
   ((Name)("name")JSON_STRING)
      )

      DECLARE_JSON_ENTITY_IMPLEMENTATION(Page,  "Page",
   ((Id)("id")JSON_INT)
      ((Name)("name")JSON_STRING)
      ((PageOrder)("pageOrder")JSON_INT)
      )

      DECLARE_JSON_ENTITY_IMPLEMENTATION(Widget,  "Widget",
   ((Id)("id")JSON_INT)
      ((IdPage)("idPage")JSON_INT)
      ((Name)("name")JSON_STRING)
      ((SizeX)("sizeX")JSON_INT)
      ((SizeY)("sizeY")JSON_INT)
      ((PositionX)("positionX")JSON_INT)
      ((PositionY)("positionY")JSON_INT)
      ((Configuration)("configuration")JSON_PTREE)
      )


      DECLARE_JSON_ENTITY_IMPLEMENTATION(PluginEventLogger,  "PluginEventLogger",
   ((Id)("id")JSON_INT)
      ((EventDate)("eventDate")JSON_DATE)
      ((PluginName)("pluginName")JSON_STRING)
      ((PluginVersion)("pluginVersion")JSON_STRING)
      ((PluginRelease)("pluginRelease")JSON_ENUM(shared::plugin::information::EReleaseType))
      ((EventType)("eventType")JSON_ENUM(database::entities::EEventType))
      ((Message)("message")JSON_STRING)
      )

      DECLARE_JSON_ENTITY_IMPLEMENTATION(EventLogger,  "EventLogger",
   ((Id)("id")JSON_INT)
      ((Date)("date")JSON_DATE)
      ((Code)("code")JSON_STRING)
      ((OptionalData)("optionalData")JSON_STRING)
      )

} //namespace json
} //namespace rest
} //namespace web 


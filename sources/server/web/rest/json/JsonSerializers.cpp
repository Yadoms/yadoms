#include "stdafx.h"

#include "JsonSerializers.h"
#include "JsonSerializersHelpers.hpp"
#include <shared/HardwarePlugin/Interfaces/IHardwarePluginInformation.h>
#include "database/entities/HardwareEventLogger.h"

DECLARE_JSON_ENTITY_IMPLEMENTATION(Hardware, "Hardware",
   ((Id)("id")JSON_INT)
   ((Name)("name")JSON_STRING)
   ((PluginName)("pluginName")JSON_STRING)
   ((Configuration)("configuration")JSON_STRING)
   ((Enabled)("enabled")JSON_BOOL)
   ((Deleted)("deleted")JSON_BOOL)
)


DECLARE_JSON_ENTITY_IMPLEMENTATION(Configuration,  "Configuration",
   ((Section)("section")JSON_STRING)
   ((Name)("name")JSON_STRING)
   ((Value)("value")JSON_STRING)
   ((DefaultValue)("default_value")JSON_STRING)
   ((Description)("description")JSON_STRING)
   ((SecurityAccess)("securityAccess")JSON_ENUM(CConfiguration::ESecurityAccess))
   ((LastModificationDate)("last_modification_date")JSON_DATE)
)


DECLARE_JSON_ENTITY_IMPLEMENTATION(Acquisition,  "Acquisition",
   ((Id)("id")JSON_INT)
   ((Source)("source")JSON_STRING)
   ((Keyword)("keyword")JSON_STRING)
   ((Value)("value")JSON_STRING)
   ((Date)("date")JSON_DATE)
)


DECLARE_JSON_ENTITY_IMPLEMENTATION(Device,  "Device",
   ((Id)("id")JSON_INT)
   ((DataSource)("data_source")JSON_STRING)
   ((Name)("name")JSON_STRING)
   ((Configuration)("configuration")JSON_STRING)
)


DECLARE_JSON_ENTITY_IMPLEMENTATION(Keyword,  "Keyword",
   ((Name)("name")JSON_STRING)
)

DECLARE_JSON_ENTITY_IMPLEMENTATION(Page,  "Page",
   ((Id)("id")JSON_INT)
   ((Name)("name")JSON_STRING)
)

DECLARE_JSON_ENTITY_IMPLEMENTATION(Widget,  "Widget",
   ((Id)("id")JSON_INT)
   ((IdPage)("idPage")JSON_INT)
   ((Name)("name")JSON_STRING)
   ((SizeX)("sizeX")JSON_INT)
   ((SizeY)("sizeY")JSON_INT)
   ((PositionX)("positionX")JSON_INT)
   ((PositionY)("positionY")JSON_INT)
   ((Configuration)("configuration")JSON_STRING)
)

DECLARE_JSON_ENTITY_IMPLEMENTATION(HardwareEventLogger,  "HardwareEventLogger",
   ((Id)("id")JSON_INT)
   ((EventDate)("eventDate")JSON_DATE)
   ((PluginName)("pluginName")JSON_STRING)
   ((PluginVersion)("pluginVersion")JSON_STRING)
   ((PluginRelease)("pluginRelease")JSON_ENUM(IHardwarePluginInformation::EReleaseType))
   ((EventType)("eventType")JSON_ENUM(CHardwareEventLogger::EEventType))
   ((Message)("message")JSON_STRING)
)


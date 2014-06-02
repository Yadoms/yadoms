#include "stdafx.h"
#include "SQLiteDatabaseTables.h"
#include "SQLiteDatabaseTablesHelpers.hpp"


namespace database { 
namespace sqlite { 

   DECLARE_TABLE_COLUMN_NAMES(Plugin, "Plugin",
      ((Id)("id"))
      ((Name)("name"))
      ((Type)("type"))
      ((Configuration)("configuration"))
      ((AutoStart)("auto_start"))
   )

   DECLARE_TABLE_COLUMN_NAMES(Configuration,  "Configuration",
      ((Section)("section"))
      ((Name)("name"))
      ((Value)("value"))
      ((DefaultValue)("default_value"))
      ((Description)("description"))
      ((SecurityAccess)("security_access"))
      ((LastModificationDate)("last_modification_date"))
   )




   DECLARE_TABLE_COLUMN_NAMES(Page,  "Page",
      ((Id)("id"))
      ((Name)("name"))
      ((PageOrder)("pageOrder"))
   )

   DECLARE_TABLE_COLUMN_NAMES(Widget,  "Widget",
      ((Id)("id"))
      ((IdPage)("idPage"))
      ((Name)("name"))
      ((SizeX)("sizeX"))
      ((SizeY)("sizeY"))
      ((PositionX)("positionX"))
      ((PositionY)("positionY"))
      ((Configuration)("configuration"))
   )

   DECLARE_TABLE_COLUMN_NAMES(PluginEventLogger, "PluginEventLogger",
      ((Id)("id"))
      ((EventDate)("eventDate"))
      ((PluginName)("pluginName"))
      ((PluginVersion)("pluginVersion"))
      ((PluginRelease)("pluginRelease"))
      ((EventType)("eventType"))
      ((Message)("message"))
   )

   DECLARE_TABLE_COLUMN_NAMES(EventLogger, "EventLogger",
      ((Id)("id"))
      ((Date)("date"))
      ((Code)("code"))
      ((Who)("who"))
      ((What)("what"))
   )



   DECLARE_TABLE_COLUMN_NAMES(Device,  "Device",
      ((Id)("id"))
      ((PluginId)("pluginId"))
      ((Name)("name"))
      ((FriendlyName)("friendlyName"))
   )

   DECLARE_TABLE_COLUMN_NAMES(Keyword,  "Keyword",
      ((Id)("id"))
      ((DeviceId)("deviceId"))
      ((CapacityName)("capacityName"))
      ((CapacityAccessMode)("capacityAccessMode"))
      ((Name)("name"))
      ((FriendlyName)("friendlyName"))
      ((Units)("units"))
      ((Minimum)("minimum"))
      ((Maximum)("maximum"))
   )

   DECLARE_TABLE_COLUMN_NAMES(Acquisition, "Acquisition",
      ((Id)("id"))
      ((Date)("date"))
      ((KeywordId)("keywordId"))
      ((Value)("value"))
   )

} //namespace sqlite
} //namespace database 


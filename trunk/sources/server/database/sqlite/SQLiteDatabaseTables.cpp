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
      ((AutoStart)("autoStart"))
   )

   DECLARE_TABLE_COLUMN_NAMES(Configuration,  "Configuration",
      ((Section)("section"))
      ((Name)("name"))
      ((Value)("value"))
      ((DefaultValue)("defaultValue"))
      ((Description)("description"))
      ((SecurityAccess)("securityAccess"))
      ((LastModificationDate)("lastModificationDate"))
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
      ((Model)("model"))
	  ((Details)("details"))
   )

   DECLARE_TABLE_COLUMN_NAMES(Keyword,  "Keyword",
      ((Id)("id"))
      ((DeviceId)("deviceId"))
      ((CapacityName)("capacityName"))
      ((AccessMode)("accessMode"))
      ((Name)("name"))
      ((FriendlyName)("friendlyName"))
      ((Type)("type"))
      ((Units)("units"))
      ((Details)("details"))
   )

   DECLARE_TABLE_COLUMN_NAMES(Acquisition, "Acquisition",
      ((Id)("id"))
      ((Date)("date"))
      ((KeywordId)("keywordId"))
      ((Value)("value"))
   )

   DECLARE_TABLE_COLUMN_NAMES(AcquisitionSummary, "AcquisitionSummary",
      ((Id)("id"))
      ((Type)("type"))
      ((Date)("date"))
      ((KeywordId)("keywordId"))
      ((Mean)("meanValue"))
      ((Min)("minValue"))
      ((Max)("maxValue"))
   )

} //namespace sqlite
} //namespace database 


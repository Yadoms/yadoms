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
      ((Category)("category"))
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
      ((Measure)("measure"))
      ((Details)("details"))
   )

   DECLARE_TABLE_COLUMN_NAMES(Acquisition, "Acquisition",
      ((Date)("date"))
      ((KeywordId)("keywordId"))
      ((Value)("value"))
   )

   DECLARE_TABLE_COLUMN_NAMES(AcquisitionSummary, "AcquisitionSummary",
      ((Type)("type"))
      ((Date)("date"))
      ((KeywordId)("keywordId"))
      ((Avg)("avgValue"))
      ((Min)("minValue"))
      ((Max)("maxValue"))
   )


   DECLARE_TABLE_COLUMN_NAMES(AutomationScript, "AutomationScript",
      ((Id)("id"))
      ((Name)("name"))
      ((Description)("description"))
      ((Triggers)("triggers"))
      ((Actions)("actions"))
   )


//   DECLARE_TABLE_COLUMN_NAMES(Contact, "Contact",
//      ((Id)("id"))
//      ((Identity)("identity"))
//      ((Information)("information"))
//   )


} //namespace sqlite
} //namespace database 


#include "stdafx.h"
#include "SQLiteDatabaseTables.h"
#include "SQLiteDatabaseTablesHelpers.hpp"


namespace database { 
namespace sqlite { 

   DECLARE_TABLE_COLUMN_NAMES(Plugin, "Plugin",
      ((Id)("id"))
      ((Name)("name"))
      ((PluginName)("pluginName"))
      ((Configuration)("configuration"))
      ((Enabled)("enabled"))
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

   DECLARE_TABLE_COLUMN_NAMES(Acquisition,  "Acquisition",
      ((Id)("id"))
      ((Source)("source"))
      ((Keyword)("keyword"))
      ((Value)("value"))
      ((Date)("date"))
   )


   DECLARE_TABLE_COLUMN_NAMES(Device,  "Device",
      ((Id)("id"))
      ((DataSource)("data_source"))
      ((Name)("name"))
      ((Configuration)("configuration"))
   )


   DECLARE_TABLE_COLUMN_NAMES(Keyword,  "Keyword",
      ((Name)("name"))
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
      ((OptionalData)("optionalData"))
   )



   DECLARE_TABLE_COLUMN_NAMES(XplMessage, "XplMessage",
      ((Id)("id"))
      ((Type)("type"))
      ((Hop)("hop"))
      ((Source)("source"))
      ((Target)("target"))
      ((MessageSchemaId)("messageSchemaId"))
      ((Date)("date"))
   )
   

   DECLARE_TABLE_COLUMN_NAMES(XplMessageContent, "XplMessageContent",
      ((Id)("id"))
      ((IdMessage)("idMessage"))
      ((Key)("key"))
      ((Value)("value"))
   )
} //namespace sqlite
} //namespace database 


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


   DECLARE_TABLE_COLUMN_NAMES(Keyword,  "Keyword",
      ((DeviceId)("deviceId"))
      ((Name)("name"))
      ((Units)("units"))
      ((Type)("type"))
      ((Minimum)("minimum"))
      ((Maximum)("maximum"))
      ((Parameters)("parameters"))
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



   DECLARE_TABLE_COLUMN_NAMES(Message, "Message",
      ((Id)("id"))
      ((Date)("date"))
      ((DeviceId)("deviceId"))
   )
   

   DECLARE_TABLE_COLUMN_NAMES(MessageContent, "MessageContent",
      ((Id)("id"))
      ((IdMessage)("idMessage"))
      ((Key)("key"))
      ((Value)("value"))
   )

   DECLARE_TABLE_COLUMN_NAMES(Capacity,  "Capacity",
      ((Id)("id"))
      ((Name)("name"))
   )

   DECLARE_TABLE_COLUMN_NAMES(InterDeviceCapacity,  "InterDeviceCapacity",
      ((DeviceId)("idDevice"))
      ((CapacityId)("idCapacity"))
      ((AccessMode)("accessMode"))
   )

   DECLARE_TABLE_COLUMN_NAMES(Device,  "Device",
      ((Id)("id"))
      ((PluginId)("pluginId"))
      ((Name)("name"))
      ((FriendlyName)("friendlyName"))
   )


} //namespace sqlite
} //namespace database 


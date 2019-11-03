#include "stdafx.h"
#include "DatabaseTables.h"
#include "DatabaseTablesHelpers.hpp"


namespace database
{
   namespace common
   {
      DECLARE_TABLE_COLUMN_NAMES(Plugin, "Plugin",
         ((Id)("id"))
         ((DisplayName)("displayName"))
         ((Type)("type"))
         ((Configuration)("configuration"))
         ((AutoStart)("autoStart"))
         ((Category)("category"))
      )

      DECLARE_TABLE_COLUMN_NAMES(Configuration, "Configuration",
         ((Section)("section"))
         ((Value)("value"))
         ((LastModificationDate)("lastModificationDate"))
      )

      DECLARE_TABLE_COLUMN_NAMES(Page, "Page",
         ((Id)("id"))
         ((Name)("name"))
         ((PageOrder)("pageOrder"))
      )

      DECLARE_TABLE_COLUMN_NAMES(Widget, "Widget",
         ((Id)("id"))
         ((IdPage)("idPage"))
         ((Type)("type"))
         ((SizeX)("sizeX"))
         ((SizeY)("sizeY"))
         ((Position)("position"))
         ((Title)("title"))
         ((Configuration)("configuration"))
      )

      DECLARE_TABLE_COLUMN_NAMES(PluginEventLogger, "PluginEventLogger",
         ((Id)("id"))
         ((EventDate)("eventDate"))
         ((PluginName)("pluginName"))
         ((PluginVersion)("pluginVersion"))
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


      DECLARE_TABLE_COLUMN_NAMES(Device, "Device",
         ((Id)("id"))
         ((PluginId)("pluginId"))
         ((Name)("name"))
         ((FriendlyName)("friendlyName"))
         ((Model)("model"))
         ((Details)("details"))
         ((Configuration)("configuration"))
         ((Type)("type"))
         ((Blacklist)("blacklist"))
      )

      DECLARE_TABLE_COLUMN_NAMES(Keyword, "Keyword",
         ((Id)("id"))
         ((DeviceId)("deviceId"))
         ((CapacityName)("capacityName"))
         ((AccessMode)("accessMode"))
         ((Name)("name"))
         ((FriendlyName)("friendlyName"))
         ((Type)("type"))
         ((Units)("units"))
         ((TypeInfo)("typeInfo"))
         ((Measure)("measure"))
         ((Details)("details"))
         ((Blacklist)("blacklist"))
         ((HistoryDepth)("historyDepth"))
         ((LastAcquisitionValue)("lastAcquisitionValue"))
         ((LastAcquisitionDate)("lastAcquisitionDate"))
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
         ((Count)("count"))
      )


      DECLARE_TABLE_COLUMN_NAMES(Rule, "Rule",
         ((Id)("id"))
         ((Name)("name"))
         ((Description)("description"))
         ((Interpreter)("interpreter"))
         ((Editor)("editor"))
         ((Model)("model"))
         ((Content)("content"))
         ((Configuration)("configuration"))
         ((AutoStart)("autoStart"))
         ((State)("state"))
         ((ErrorMessage)("errorMessage"))
         ((StartDate)("startDate"))
         ((StopDate)("stopDate"))
      )


      DECLARE_TABLE_COLUMN_NAMES(Recipient, "Recipient",
         ((Id)("id"))
         ((FirstName)("firstName"))
         ((LastName)("lastName"))
      )


      DECLARE_TABLE_COLUMN_NAMES(RecipientField, "RecipientField",
         ((IdRecipient)("idRecipient"))
         ((PluginType)("pluginType"))
         ((FieldName)("fieldName"))
         ((Value)("value"))
      )
   } //namespace common
} //namespace database 

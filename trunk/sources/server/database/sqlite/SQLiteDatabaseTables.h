#pragma once

#include "SQLiteDatabaseTablesHelpers.hpp"
#include "database/entities/Entities.h"


namespace database { 
namespace sqlite { 

   DECLARE_TABLE(Plugin,
      (Id)
      (Name)
      (PluginName)
      (Configuration)
      (Enabled)
      (Deleted)
   )

   DECLARE_TABLE(Configuration,
      (Section)
      (Name)
      (Value)
      (DefaultValue)
      (Description)
      (SecurityAccess)
      (LastModificationDate)
   )


   DECLARE_TABLE(Acquisition,
      (Id)
      (Source)
      (Keyword)
      (Value)
      (Date)
   )


   DECLARE_TABLE(Device,
      (Id)
      (DataSource)
      (Name)
      (Configuration)
   )

   DECLARE_TABLE(Keyword,
      (Name)
   )


   DECLARE_TABLE(Page,
      (Id)
      (Name)
      (PageOrder)
   )

   DECLARE_TABLE(Widget,
      (Id)
      (IdPage)
      (Name)
      (SizeX)
      (SizeY)
      (PositionX)
      (PositionY)
      (Configuration)
   )

   DECLARE_TABLE(PluginEventLogger,
      (Id)
      (EventDate)
      (PluginName)
      (PluginVersion)
      (PluginRelease)
      (EventType)
      (Message)
   )

   DECLARE_TABLE(EventLogger,
      (Id)
      (EventDate)
      (EventType)
      (OptionalData)
   )


} //namespace sqlite
} //namespace database 


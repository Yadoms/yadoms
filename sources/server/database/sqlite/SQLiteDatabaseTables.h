#pragma once

#include "SQLiteDatabaseTablesHelpers.hpp"
#include "database/entities/Entities.h"


namespace database { 
namespace sqlite { 

   DECLARE_TABLE(Plugin,
      (Id)
      (Name)
      (Type)
      (Configuration)
      (AutoStart)
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


   DECLARE_TABLE(Keyword,
      (DeviceId)
      (Name)
      (Units)
      (Type)
      (Minimum)
      (Maximum)
      (Parameters)
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
      (Date)
      (Code)
      (Who)
      (What)
   )

   DECLARE_TABLE(Message,
      (Id)
      (Date)
      (DeviceId)
   )   

   DECLARE_TABLE(MessageContent,
      (Id)
      (IdMessage)
      (Key)
      (Value)
   )   
   
   DECLARE_TABLE(Device,
      (Id)
      (Address)
      (Protocol)
      (HardwareIdentifier)
      (Name)
   )
} //namespace sqlite
} //namespace database 


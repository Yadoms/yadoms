#pragma once

#include "DatabaseColumn.h"
#include "DatabaseTablesHelpers.hpp"
#include "database/entities/Entities.h"


namespace database {
namespace common {

   DECLARE_TABLE(Plugin,
      (Id)
      (DisplayName)
      (Type)
      (Configuration)
      (AutoStart)
      (Category)
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




   DECLARE_TABLE(Page,
      (Id)
      (Name)
      (PageOrder)
   )

   DECLARE_TABLE(Widget,
      (Id)
      (IdPage)
      (Type)
      (SizeX)
      (SizeY)
      (Position)
      (Title)
      (Configuration)
   )

   DECLARE_TABLE(PluginEventLogger,
      (Id)
      (EventDate)
      (PluginName)
      (PluginVersion)
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

   DECLARE_TABLE(Device,
      (Id)
      (PluginId)
      (Name)
      (FriendlyName)
      (Model)
      (Details)
      (Configuration)
      (Type)
      (Blacklist)
   )

   DECLARE_TABLE(Keyword,
      (Id)
      (DeviceId)
      (CapacityName)
      (AccessMode)
      (Name)
      (FriendlyName)
      (Type)
      (Units)
      (TypeInfo)
      (Measure)
      (Details)
      (Blacklist)
      (LastAcquisitionValue)
      (LastAcquisitionDate)
   )

   DECLARE_TABLE(Acquisition,
      (Date)
      (KeywordId)
      (Value)
   )

   DECLARE_TABLE(AcquisitionSummary,
      (Type)
      (Date)
      (KeywordId)
      (Avg)
      (Min)
      (Max)
   )   

   DECLARE_TABLE(Rule,
      (Id)
      (Name)
      (Description)
      (Interpreter)
      (Editor)
      (Model)
      (Content)
      (Configuration)
      (AutoStart)
      (State)
      (ErrorMessage)
      (StartDate)
      (StopDate)
   )   

   DECLARE_TABLE(Recipient,
      (Id)
      (FirstName)
      (LastName)
   )

   DECLARE_TABLE(RecipientField,
      (IdRecipient)
      (PluginType)
      (FieldName)
      (Value)
   )


} //namespace common
} //namespace database 


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

   DECLARE_TABLE(Device,
      (Id)
      (PluginId)
      (Name)
      (FriendlyName)
      (Model)
	  (Details)
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
      (Measure)
      (Details)
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
      (Triggers)
      (Actions)
      (Enable)
   )   

	DECLARE_TABLE(Recipient,
	   (Id)
		(FirstName)
		(LastName)
	)   

	DECLARE_TABLE(Field,
	   (Id)
		(Name)
		(Category)
		(VerificationRegex)
	)   

	DECLARE_TABLE(RecipientFields,
	   (IdRecipient)
		(IdField)
		(Value)
	)   


} //namespace sqlite
} //namespace database 


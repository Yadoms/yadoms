#pragma once

#include "IEntitySerializer.h"
#include "IEntityDeserializer.h"
#include "JsonSerializersHelpers.hpp"
#include "database/entities/Entities.h"
#include "JsonDate.h"

namespace web { namespace rest { namespace json {

   DECLARE_JSON_ENTITY_HEADER(Plugin,
      (Id)
      (Name)
      (PluginName)
      (Configuration)
      (Enabled)
      )

   DECLARE_JSON_ENTITY_HEADER(Configuration,
      (Section)
      (Name)
      (Value)
      (DefaultValue)
      (Description)
      (SecurityAccess)
      (LastModificationDate)
      )

   DECLARE_JSON_ENTITY_HEADER(Device,
      (Id)
      (Address)
      (Protocol)
      (Name)
      (HardwareIdentifier)
      )


   DECLARE_JSON_ENTITY_HEADER(Keyword,
      (DeviceId)
      (Name)
      (Units)
      (Type)
      (Minimum)
      (Maximum)
      (Parameters)
   )


   DECLARE_JSON_ENTITY_HEADER(Page,
      (Id)
      (Name)
      (PageOrder)
   )

   DECLARE_JSON_ENTITY_HEADER(Widget,
      (Id)
      (IdPage)
      (Name)
      (SizeX)
      (SizeY)
      (PositionX)
      (PositionY)
      (Configuration)
   )


   DECLARE_JSON_ENTITY_HEADER(PluginEventLogger,
      (Id)
      (EventDate)
      (PluginName)
      (PluginVersion)
      (PluginRelease)
      (EventType)
      (Message)
   )

   DECLARE_JSON_ENTITY_HEADER(EventLogger,
      (Id)
      (Date)
      (Code)
      (Who)
      (What)
   )




} //namespace json
} //namespace rest
} //namespace web 


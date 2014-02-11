#pragma once

#include "IEntitySerializer.h"
#include "IEntityDeserializer.h"
#include "JsonSerializersHelpers.hpp"
#include "database/entities/Acquisition.h"
#include "database/entities/Configuration.h"
#include "database/entities/Device.h"
#include "database/entities/Hardware.h"
#include "database/entities/Keyword.h"
#include "database/entities/Page.h"
#include "database/entities/Widget.h"
#include "database/entities/HardwareEventLogger.h"

DECLARE_JSON_ENTITY_HEADER(Hardware,
   (Id)
   (Name)
   (PluginName)
   (Configuration)
   (Enabled)
   (Deleted)
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

DECLARE_JSON_ENTITY_HEADER(Acquisition,
   (Id)
   (Source)
   (Keyword)
   (Value)
   (Date)
)


DECLARE_JSON_ENTITY_HEADER(Device,
   (Id)
   (DataSource)
   (Name)
   (Configuration)
)


DECLARE_JSON_ENTITY_HEADER(Keyword,
   (Name)
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


DECLARE_JSON_ENTITY_HEADER(HardwareEventLogger,
   (Id)
   (EventDate)
   (PluginName)
   (PluginVersion)
   (PluginRelease)
   (EventType)
   (Message)
)

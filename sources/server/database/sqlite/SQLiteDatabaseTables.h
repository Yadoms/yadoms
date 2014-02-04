#pragma once

#include "SQLiteDatabaseTablesHelpers.hpp"
#include "database/entities/Acquisition.h"
#include "database/entities/Configuration.h"
#include "database/entities/Device.h"
#include "database/entities/Hardware.h"
#include "database/entities/Keyword.h"

DECLARE_TABLE(Hardware, 
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




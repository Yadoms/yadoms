#pragma once

#include "SQLiteDatabaseTablesHelpers.hpp"

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


DECLARE_TABLE(Acquisitions, 
   (Id)
   (Source)
   (Keyword)
   (Value)
   (Date)
)



#include "stdafx.h"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/SQLiteDatabaseTablesHelpers.hpp"


DECLARE_STATIC_TABLE_CREATION_SCRIPT(Acquisition, "CREATE TABLE Acquisition\
                                                   (\
                                                      id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
                                                      source VARCHAR(50) NOT NULL,\
                                                      keyword TEXT NOT NULL,\
                                                      value TEXT NOT NULL,\
                                                      date DATETIME NOT NULL\
                                                   )")

DECLARE_STATIC_TABLE_CREATION_SCRIPT(Configuration, " CREATE TABLE Configuration\
                                                      (\
                                                         section VARCHAR(50) NOT NULL,\
                                                         name VARCHAR(50) NOT NULL,\
                                                         value TEXT NOT NULL,\
                                                         default_value TEXT,\
                                                         description TEXT,\
                                                         last_modification_date DATETIME,\
                                                         PRIMARY KEY(section,name)\
                                                      )")

DECLARE_STATIC_TABLE_CREATION_SCRIPT(Device, "  CREATE TABLE Device\
                                                (\
                                                   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
                                                   data_source VARCHAR(50) NOT NULL,\
                                                   name TEXT NOT NULL,\
                                                   configuration TEXT\
                                                )")


DECLARE_STATIC_TABLE_CREATION_SCRIPT(Hardware, "CREATE TABLE Hardware\
                                                (  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
                                                   name TEXT NOT NULL,\
                                                   pluginName TEXT NOT NULL,\
                                                   configuration  TEXT, \
                                                   deleted  INTEGER DEFAULT 0, \
                                                   enabled  INTEGER DEFAULT 1\
                                                )")

DECLARE_STATIC_TABLE_CREATION_SCRIPT(Keyword, " CREATE TABLE Keyword\
                                                (\
                                                         name TEXT NOT NULL,\
                                                         PRIMARY KEY(name)\
                                                )")
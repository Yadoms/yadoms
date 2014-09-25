#include "stdafx.h"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/SQLiteDatabaseTablesHelpers.hpp"


   namespace database { 
      namespace sqlite { 

DECLARE_STATIC_TABLE_CREATION_SCRIPT(Configuration, " CREATE TABLE Configuration                                  \
                                                      (  section VARCHAR(50) NOT NULL,                            \
                                                         name VARCHAR(50) NOT NULL,                               \
                                                         value TEXT NOT NULL,                                     \
                                                         defaultValue TEXT,                                       \
                                                         description TEXT,                                        \
                                                         securityAccess  TEXT DEFAULT \"none\",                       \
                                                         lastModificationDate TEXT,                               \
                                                         PRIMARY KEY(section,name)                                \
                                                      )")



DECLARE_STATIC_TABLE_CREATION_SCRIPT(Plugin, "CREATE TABLE Plugin                                                 \
                                                (  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,                 \
                                                   name TEXT NOT NULL,                                            \
                                                   type TEXT NOT NULL,                                            \
                                                   configuration  TEXT,                                           \
                                                   autoStart  INTEGER DEFAULT 1                                   \
                                                )")



DECLARE_STATIC_TABLE_CREATION_SCRIPT(Page, " CREATE TABLE Page                                                    \
                                                (                                                                 \
                                                   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,                 \
                                                   name TEXT NOT NULL,                                            \
                                                   pageOrder  INTEGER DEFAULT 0                                   \
                                                )")

DECLARE_STATIC_TABLE_CREATION_SCRIPT(Widget, "CREATE TABLE Widget                                                 \
                                                (  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,                 \
                                                   idPage  INTEGER NOT NULL,                                      \
                                                   name TEXT NOT NULL,                                            \
                                                   sizeX  INTEGER DEFAULT 1,                                      \
                                                   sizeY  INTEGER DEFAULT 1,                                      \
                                                   positionX  INTEGER DEFAULT 1,                                  \
                                                   positionY  INTEGER DEFAULT 1,                                  \
                                                   configuration  TEXT                                            \
                                                )")



DECLARE_STATIC_TABLE_CREATION_SCRIPT(PluginEventLogger,   "CREATE TABLE PluginEventLogger                         \
                                                            (  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,     \
                                                               eventDate  TEXT NOT NULL,                          \
                                                               pluginName TEXT NOT NULL,                          \
                                                               pluginVersion TEXT NOT NULL,                       \
                                                               pluginRelease INTEGER,                             \
                                                               eventType  TEXT DEFAULT \"unload\",                \
                                                               message  TEXT                                      \
                                                            )")



DECLARE_STATIC_TABLE_CREATION_SCRIPT(EventLogger, "CREATE TABLE EventLogger                                       \
                                                            (  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,     \
                                                               date  TEXT NOT NULL,                               \
                                                               code TEXT NOT NULL DEFAULT \"defaultcode\",        \
                                                               who  TEXT,                                         \
                                                               what TEXT                                          \
                                                            )")

                                                            
                                                                                        
DECLARE_STATIC_TABLE_CREATION_SCRIPT(Device, "  CREATE TABLE Device                                               \
                                                (                                                                 \
                                                   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,                 \
                                                   pluginId INTEGER NOT NULL,                                     \
                                                   name TEXT NOT NULL,                                            \
                                                   friendlyName TEXT NOT NULL,                                    \
                                                   model TEXT,                                                    \
                                                   details TEXT                                                   \
                                                )")

DECLARE_STATIC_TABLE_CREATION_SCRIPT(Keyword, " CREATE TABLE Keyword                                              \
                                                (                                                                 \
                                                   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,                 \
                                                   deviceId INTEGER NOT NULL,                                     \
                                                   capacityName TEXT NOT NULL,                                    \
                                                   accessMode TEXT NOT NULL,                                      \
                                                   name TEXT NOT NULL,                                            \
                                                   friendlyName TEXT,                                             \
                                                   type TEXT NOT NULL,                                            \
                                                   units TEXT,                                                    \
                                                   details TEXT                                                   \
                                                )")                                                               


DECLARE_STATIC_TABLE_CREATION_SCRIPT(Acquisition,    "CREATE TABLE Acquisition                                    \
                                                      (  date TEXT NOT NULL,                                      \
                                                         keywordId INTEGER NOT NULL,                              \
                                                         value TEXT NOT NULL,                                     \
                                                         PRIMARY KEY (date, keywordId)                            \
                                                      )")

DECLARE_STATIC_TABLE_CREATION_SCRIPT(AcquisitionSummary,    "CREATE TABLE AcquisitionSummary                      \
                                                            (  type TEXT NOT NULL,                                \
                                                               date TEXT NOT NULL,                                \
                                                               keywordId INTEGER NOT NULL,                        \
                                                               avgValue REAL NOT NULL,                            \
                                                               minValue REAL NOT NULL,                            \
                                                               maxValue REAL NOT NULL,                            \
                                                               PRIMARY KEY (type, date, keywordId)                \
                                                               )")                           

DECLARE_STATIC_INDEXES_CREATION_SCRIPT(Acquisition, "CREATE INDEX acqByDate ON Acquisition (date, keywordId)" )



      } //namespace sqlite
   } //namespace database 


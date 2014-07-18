#include "stdafx.h"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/SQLiteDatabaseTablesHelpers.hpp"


   namespace database { 
      namespace sqlite { 

DECLARE_STATIC_TABLE_CREATION_SCRIPT(Configuration, " CREATE TABLE Configuration                                  \
                                                      (  section VARCHAR(50) NOT NULL,                            \
                                                         name VARCHAR(50) NOT NULL,                               \
                                                         value TEXT NOT NULL,                                     \
                                                         defaultValue TEXT,                                      \
                                                         description TEXT,                                        \
                                                         securityAccess  INTEGER DEFAULT 0,                      \
                                                         lastModificationDate DATETIME,                         \
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
                                                               eventDate  DATETIME NOT NULL,                      \
                                                               pluginName TEXT NOT NULL,                          \
                                                               pluginVersion TEXT NOT NULL,                       \
                                                               pluginRelease INTEGER,                             \
                                                               eventType  INTEGER DEFAULT 0,                      \
                                                               message  TEXT                                      \
                                                            )")



DECLARE_STATIC_TABLE_CREATION_SCRIPT(EventLogger, "CREATE TABLE EventLogger                                       \
                                                            (  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,     \
                                                               date  DATETIME NOT NULL,                           \
                                                               code INTEGERNOT NULL DEFAULT 0,                    \
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
                                                   accessMode INTEGER NOT NULL,                                   \
                                                   name TEXT NOT NULL,                                            \
                                                   friendlyName TEXT,                                             \
                                                   type INTEGER NOT NULL,                                         \
                                                   units TEXT,                                                    \
                                                   details TEXT                                                   \
                                                )")                                                               


DECLARE_STATIC_TABLE_CREATION_SCRIPT(Acquisition,    "CREATE TABLE Acquisition                                    \
                                                      (  id INTEGER NOT NULL PRIMARY KEY  AUTOINCREMENT,          \
                                                         date DATETIME NOT NULL,                                  \
                                                         keywordId INTEGER NOT NULL,                              \
                                                         value TEXT NOT NULL                                      \
                                                         )")                           
                                                            

      } //namespace sqlite
   } //namespace database 


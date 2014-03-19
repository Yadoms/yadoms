#include "stdafx.h"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/SQLiteDatabaseTablesHelpers.hpp"


   namespace database { 
      namespace sqlite { 

DECLARE_STATIC_TABLE_CREATION_SCRIPT(Acquisition, "CREATE TABLE Acquisition                                       \
                                                   (  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,              \
                                                      source VARCHAR(50) NOT NULL,                                \
                                                      keyword TEXT NOT NULL,                                      \
                                                      value TEXT NOT NULL,                                        \
                                                      date DATETIME NOT NULL                                      \
                                                   )")

DECLARE_STATIC_TABLE_CREATION_SCRIPT(Configuration, " CREATE TABLE Configuration                                  \
                                                      (  section VARCHAR(50) NOT NULL,                            \
                                                         name VARCHAR(50) NOT NULL,                               \
                                                         value TEXT NOT NULL,                                     \
                                                         default_value TEXT,                                      \
                                                         description TEXT,                                        \
                                                         security_access  INTEGER DEFAULT 0,                       \
                                                         last_modification_date DATETIME,                         \
                                                         PRIMARY KEY(section,name)                                \
                                                      )")

DECLARE_STATIC_TABLE_CREATION_SCRIPT(Device, "  CREATE TABLE Device                                               \
                                                (                                                                 \
                                                   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,                 \
                                                   data_source VARCHAR(50) NOT NULL,                              \
                                                   name TEXT NOT NULL,                                            \
                                                   configuration TEXT                                             \
                                                )")


DECLARE_STATIC_TABLE_CREATION_SCRIPT(Plugin, "CREATE TABLE Plugin                                             \
                                                (  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,                 \
                                                   name TEXT NOT NULL,                                            \
                                                   pluginName TEXT NOT NULL UNIQUE,                               \
                                                   configuration  TEXT,                                           \
                                                   enabled  INTEGER DEFAULT 1                                     \
                                                )")

DECLARE_STATIC_TABLE_CREATION_SCRIPT(Keyword, " CREATE TABLE Keyword                                              \
                                                (                                                                 \
                                                         name TEXT NOT NULL,                                      \
                                                         PRIMARY KEY(name)                                        \
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



DECLARE_STATIC_TABLE_CREATION_SCRIPT(PluginEventLogger, "CREATE TABLE PluginEventLogger                         \
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
                                                               code TEXT NOT NULL,                                \
                                                               optionalData  TEXT                                 \
                                                            )")

                                                            
                                                            
DECLARE_STATIC_TABLE_CREATION_SCRIPT(XplMessage, "CREATE TABLE XplMessage                                         \
                                                      (  id INTEGER NOT NULL PRIMARY KEY  AUTOINCREMENT,          \
                                                         type TEXT NOT NULL ,                                     \
                                                         hop INTEGER NOT NULL DEFAULT 1,                          \
                                                         source TEXT NOT NULL,                                    \
                                                         target TEXT NOT NULL ,                                   \
                                                         messageSchemaId TEXT NOT NULL,                           \
                                                         date DATETIME NOT NULL                                   \
                                                         )")                           

DECLARE_STATIC_TABLE_CREATION_SCRIPT(XplMessageContent, "CREATE TABLE XplMessageContent                           \
                                                         (  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,        \
                                                            idMessage INTEGER NOT NULL,                           \
                                                            key TEXT NOT NULL ,                                   \
                                                            value TEXT NOT NULL                                   \
                                                         )")                                                         
                                                            
                                                                                                                             
      } //namespace sqlite
   } //namespace database 


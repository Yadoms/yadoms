#include "stdafx.h"
#include "SQLiteTableCreationScriptProvider.h"

namespace database { namespace sqlite {


   CSQLiteTableCreationScriptProvider::CSQLiteTableCreationScriptProvider()
   {
   }

   CSQLiteTableCreationScriptProvider::~CSQLiteTableCreationScriptProvider()
   {
   }

   std::string CSQLiteTableCreationScriptProvider::getTableConfiguration()
   {
      return " CREATE TABLE Configuration                                  \
               (  section VARCHAR(50) NOT NULL,                            \
                  value TEXT NOT NULL,                                     \
                  lastModificationDate TEXT,                               \
                  PRIMARY KEY(section)                                     \
               )";
   }

   std::string CSQLiteTableCreationScriptProvider::getTableDevice()
   {
      return "  CREATE TABLE Device                                               \
               (                                                                 \
                  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,                 \
                  pluginId INTEGER NOT NULL,                                     \
                  name TEXT NOT NULL,                                            \
                  friendlyName TEXT NOT NULL,                                    \
                  model TEXT,                                                    \
                  details TEXT                                                   \
               )";
   }

   std::string CSQLiteTableCreationScriptProvider::getTablePlugin()
   {
      return  "CREATE TABLE Plugin                                               \
               (  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,                 \
                  displayName TEXT NOT NULL,                                     \
                  type TEXT NOT NULL,                                            \
                  configuration  TEXT,                                           \
                  autoStart  INTEGER DEFAULT 1,                                  \
                  category TEXT NOT NULL DEFAULT \"user\"                        \
               )";
   }

   std::string CSQLiteTableCreationScriptProvider::getTableKeyword()
   {
      return " CREATE TABLE Keyword                                              \
               (                                                                 \
                  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,                 \
                  deviceId INTEGER NOT NULL,                                     \
                  capacityName TEXT NOT NULL,                                    \
                  accessMode TEXT NOT NULL,                                      \
                  name TEXT NOT NULL,                                            \
                  friendlyName TEXT,                                             \
                  type TEXT NOT NULL,                                            \
                  units TEXT,                                                    \
                  typeInfo TEXT,												  \
                  measure TEXT NOT NULL,                                         \
                  details TEXT                                                   \
               )";
   }

   std::string CSQLiteTableCreationScriptProvider::getTablePage()
   {
      return " CREATE TABLE Page                                                 \
               (                                                                 \
                  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,                 \
                  name TEXT NOT NULL,                                            \
                  pageOrder  INTEGER DEFAULT 0                                   \
               )";
   }

   std::string CSQLiteTableCreationScriptProvider::getTableWidget()
   {
      return " CREATE TABLE Widget                                               \
               (  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,                 \
                  idPage  INTEGER NOT NULL,                                      \
                  type TEXT NOT NULL,                                            \
                  sizeX  INTEGER DEFAULT 1,                                      \
                  sizeY  INTEGER DEFAULT 1,                                      \
                  position  INTEGER DEFAULT 0,                                   \
                  title  TEXT,                                                   \
                  configuration  TEXT                                            \
               )";
   }

   std::string CSQLiteTableCreationScriptProvider::getTablePluginEventLogger()
   {
      return "CREATE TABLE PluginEventLogger                         \
               (  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,     \
                  eventDate  TEXT NOT NULL,                          \
                  pluginName TEXT NOT NULL,                          \
                  pluginVersion TEXT NOT NULL,                       \
                  pluginRelease INTEGER,                             \
                  eventType  TEXT DEFAULT \"unload\",                \
                  message  TEXT                                      \
               )";
   }

   std::string CSQLiteTableCreationScriptProvider::getTableEventLogger()
   {
      return "CREATE TABLE EventLogger                               \
               (  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,     \
                  date  TEXT NOT NULL,                               \
                  code TEXT NOT NULL DEFAULT \"defaultcode\",        \
                  who  TEXT,                                         \
                  what TEXT                                          \
               )";
   }

   std::string CSQLiteTableCreationScriptProvider::getTableAcquisition()
   {
      return " CREATE TABLE Acquisition                                    \
               (  date TEXT NOT NULL,                                      \
                  keywordId INTEGER NOT NULL,                              \
                  value TEXT NOT NULL,                                     \
                  PRIMARY KEY (date, keywordId)                            \
               )WITHOUT ROWID";
   }

   std::string CSQLiteTableCreationScriptProvider::getTableAcquisitionSummary()
   {
      return " CREATE TABLE AcquisitionSummary                       \
               (  type TEXT NOT NULL,                                \
                  date TEXT NOT NULL,                                \
                  keywordId INTEGER NOT NULL,                        \
                  avgValue REAL NOT NULL,                            \
                  minValue REAL NOT NULL,                            \
                  maxValue REAL NOT NULL,                            \
                  PRIMARY KEY (type, date, keywordId)                \
                  )WITHOUT ROWID";
   }

   std::string CSQLiteTableCreationScriptProvider::getTableRule()
   {
      return "CREATE TABLE Rule                                                     \
               (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,                      \
               name TEXT NOT NULL,                                                  \
               description TEXT,                                                    \
               interpreter TEXT NOT NULL,                                           \
               editor TEXT NOT NULL,                                                \
               model TEXT,                                                          \
               content TEXT,                                                        \
               configuration TEXT,                                                  \
               autoStart INTEGER DEFAULT 1,                                         \
               state TEXT NOT NULL default 'Stopped',                               \
               errorMessage TEXT,                                                   \
               startDate TEXT,                                                      \
               stopDate TEXT)";
   }

   std::string CSQLiteTableCreationScriptProvider::getTableRecipient()
   {
      return " CREATE TABLE Recipient                                \
               (  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,     \
                  firstName TEXT NOT NULL,                           \
                  lastName TEXT)";
   }

   std::string CSQLiteTableCreationScriptProvider::getTableRecipientField()
   {
      return " CREATE TABLE RecipientField									\
               (  idRecipient INTEGER NOT NULL,								\
                  pluginType TEXT NOT NULL,									\
                  fieldName TEXT NOT NULL,									\
                  value TEXT,                                        \
                  PRIMARY KEY (idRecipient, pluginType, fieldName))";
   }

   void CSQLiteTableCreationScriptProvider::getTableAcquisitionIndexes(std::vector<std::string> & indexScripts)
   {
      indexScripts.clear();
      indexScripts.push_back("CREATE INDEX acqKeywordIdIndex ON Acquisition(keywordId)");
      indexScripts.push_back("create index if not exists acqKeywordIdDateIndex on Acquisition(keywordId,date)");
   }
 
} //namespace sqlite
} //namespace database 

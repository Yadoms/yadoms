#include "stdafx.h"
#include "PgsqlTableCreationScriptProvider.h"

namespace database
{
   namespace pgsql
   {
      std::string CPgsqlTableCreationScriptProvider::getTableConfiguration()
      {
         return " CREATE TABLE Configuration                            \
              (   section VARCHAR(50) NOT NULL,                         \
                  value TEXT NOT NULL,                                  \
                  lastModificationDate TEXT,                            \
                  PRIMARY KEY(section)                                  \
            )";
      }

      std::string CPgsqlTableCreationScriptProvider::getTableDevice()
      {
         return "  CREATE TABLE Device                                        \
               (                                                              \
                  id SERIAL NOT NULL PRIMARY KEY,                             \
                  pluginId INTEGER NOT NULL,                                  \
                  name TEXT NOT NULL,                                         \
                  friendlyName TEXT NOT NULL,                                 \
                  model TEXT,                                                 \
                  details TEXT                                                \
               )";
      }

      std::string CPgsqlTableCreationScriptProvider::getTablePlugin()
      {
         return "CREATE TABLE Plugin                                             \
               (  id SERIAL NOT NULL PRIMARY KEY,                                \
                  displayName TEXT NOT NULL,                                     \
                  type TEXT NOT NULL,                                            \
                  configuration  TEXT,                                           \
                  autoStart  INTEGER DEFAULT 1,                                  \
                  category TEXT NOT NULL DEFAULT 'user'                          \
               )";
      }

      std::string CPgsqlTableCreationScriptProvider::getTableKeyword()
      {
         return " CREATE TABLE Keyword                                           \
               (                                                                 \
                  id SERIAL NOT NULL PRIMARY KEY,                                \
                  deviceId INTEGER NOT NULL,                                     \
                  capacityName TEXT NOT NULL,                                    \
                  accessMode TEXT NOT NULL,                                      \
                  name TEXT NOT NULL,                                            \
                  friendlyName TEXT,                                             \
                  type TEXT NOT NULL,                                            \
                  units TEXT,                                                    \
                  typeInfo TEXT,                                                 \
                  measure TEXT NOT NULL,                                         \
                  details TEXT                                                   \
               )";
      }

      std::string CPgsqlTableCreationScriptProvider::getTablePage()
      {
         return " CREATE TABLE Page                                              \
               (                                                                 \
                  id SERIAL NOT NULL PRIMARY KEY,                                \
                  name TEXT NOT NULL,                                            \
                  pageOrder  INTEGER DEFAULT 0                                   \
               )";
      }

      std::string CPgsqlTableCreationScriptProvider::getTableWidget()
      {
         return " CREATE TABLE Widget                                            \
               (  id SERIAL NOT NULL PRIMARY KEY,                                \
                  idPage  INTEGER NOT NULL,                                      \
                  type TEXT NOT NULL,                                            \
                  sizeX  INTEGER DEFAULT 1,                                      \
                  sizeY  INTEGER DEFAULT 1,                                      \
                  position  INTEGER DEFAULT 0,                                   \
                  title  TEXT,                                                   \
                  configuration  TEXT                                            \
               )";
      }

      std::string CPgsqlTableCreationScriptProvider::getTablePluginEventLogger()
      {
         return "CREATE TABLE PluginEventLogger                      \
               (  id SERIAL NOT NULL PRIMARY KEY,                    \
                  eventDate  TEXT NOT NULL,                          \
                  pluginName TEXT NOT NULL,                          \
                  pluginVersion TEXT NOT NULL,                       \
                  pluginRelease TEXT,                                \
                  eventType  TEXT DEFAULT 'unload',                  \
                  message  TEXT                                      \
               )";
      }

      std::string CPgsqlTableCreationScriptProvider::getTableEventLogger()
      {
         return "CREATE TABLE EventLogger                            \
               (  id SERIAL NOT NULL PRIMARY KEY,                    \
                  date  TEXT NOT NULL,                               \
                  code TEXT NOT NULL DEFAULT 'defaultcode',          \
                  who  TEXT,                                         \
                  what TEXT                                          \
               )";
      }

      std::string CPgsqlTableCreationScriptProvider::getTableAcquisition()
      {
         return " CREATE TABLE Acquisition                                 \
               (  date TEXT NOT NULL,                                      \
                  keywordId INTEGER NOT NULL,                              \
                  value TEXT NOT NULL,                                     \
                  PRIMARY KEY (date, keywordId)                            \
               )";
      }

      std::string CPgsqlTableCreationScriptProvider::getTableAcquisitionSummary()
      {
         return " CREATE TABLE AcquisitionSummary                    \
               (  type TEXT NOT NULL,                                \
                  date TEXT NOT NULL,                                \
                  keywordId INTEGER NOT NULL,                        \
                  avgValue REAL NOT NULL,                            \
                  minValue REAL NOT NULL,                            \
                  maxValue REAL NOT NULL,                            \
                  PRIMARY KEY (type, date, keywordId)                \
               )";
      }

      std::string CPgsqlTableCreationScriptProvider::getTableRule()
      {
         return "CREATE TABLE Rule                                                  \
               (SERIAL INTEGER NOT NULL PRIMARY KEY,                                \
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

      std::string CPgsqlTableCreationScriptProvider::getTableRecipient()
      {
         return "  CREATE TABLE Recipient                               \
                (  id SERIAL NOT NULL PRIMARY KEY,                      \
                   firstName TEXT NOT NULL,                             \
                   lastName TEXT)";
      }

      std::string CPgsqlTableCreationScriptProvider::getTableRecipientField()
      {
         return " CREATE TABLE RecipientField                        \
               (  idRecipient INTEGER NOT NULL,                      \
                  pluginType TEXT NOT NULL,                          \
                  fieldName TEXT NOT NULL,                           \
                  value TEXT,                                        \
                  PRIMARY KEY (idRecipient, pluginType, fieldName))";
      }

      std::string CPgsqlTableCreationScriptProvider::getTableSerialPort()
      {
         return
            "CREATE TABLE SerialPort                                 \
               (                                                     \
                  id SERIAL NOT NULL PRIMARY KEY,                    \
                  adapterKind TEXT NOT NULL DEFAULT 'Physical',      \
                  adapterParameters TEXT,                            \
                  lastKnownSerialPortPath TEXT NOT NULL              \
               )";
      }

      void CPgsqlTableCreationScriptProvider::getTableAcquisitionIndexes(std::vector<std::string>& indexScripts)
      {
         indexScripts.clear();
         //TODO : créer les index pour pgsql
         //indexScripts.push_back("CREATE INDEX acqKeywordIdIndex ON Acquisition(keywordId)");
         //indexScripts.push_back("create index if not exists acqKeywordIdDateIndex on Acquisition(keywordId,date)");
      }
   } //namespace pgsql
} //namespace database 



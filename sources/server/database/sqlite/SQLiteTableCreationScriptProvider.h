#pragma once

#include "database/ITableCreationScriptProvider.h"


namespace database
{
   namespace sqlite
   {
      class CSQLiteTableCreationScriptProvider : public ITableCreationScriptProvider
      {
      public:
         CSQLiteTableCreationScriptProvider() = default;
         virtual ~CSQLiteTableCreationScriptProvider() = default;

         // ITableCreationScriptProvider implementation
         std::string getTableConfiguration() override;
         std::string getTableDevice() override;
         std::string getTablePlugin() override;
         std::string getTableKeyword() override;
         std::string getTablePage() override;
         std::string getTableWidget() override;
         std::string getTablePluginEventLogger() override;
         std::string getTableEventLogger() override;
         std::string getTableAcquisition() override;
         std::string getTableAcquisitionSummary() override;
         std::string getTableRule() override;
         std::string getTableRecipient() override;
         std::string getTableRecipientField() override;
         std::string getTableSerialPort() override;
         void getTableAcquisitionIndexes(std::vector<std::string>& indexScripts) override;
         // [END] ITableCreationScriptProvider implementation
      };
   } //namespace sqlite
} //namespace database 

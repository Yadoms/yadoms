#pragma once

#include "database/ITableCreationScriptProvider.h"


namespace database
{
   namespace pgsql
   {
      class CPgsqlTableCreationScriptProvider : public ITableCreationScriptProvider
      {
      public:
         //--------------------------------------------------------------
         /// \brief       Constructor
         //--------------------------------------------------------------
         CPgsqlTableCreationScriptProvider();

         //--------------------------------------------------------------
         /// \brief       Destructor
         //--------------------------------------------------------------
         virtual ~CPgsqlTableCreationScriptProvider();

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
         void getTableAcquisitionIndexes(std::vector<std::string>& indexScripts) override;
         // [END] ITableCreationScriptProvider implementation
      };
   } //namespace pgsql
} //namespace database 



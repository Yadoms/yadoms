#pragma once

namespace database { 

   class ITableCreationScriptProvider
   {
   public:
      //--------------------------------------------------------------
      /// \brief           Get the creation script for table Configuration
      /// \return          The creation script
      //--------------------------------------------------------------
      virtual std::string getTableConfiguration() = 0;

      //--------------------------------------------------------------
      /// \brief           Get the creation script for table Device
      /// \return          The creation script
      //--------------------------------------------------------------
      virtual std::string getTableDevice() = 0;

      //--------------------------------------------------------------
      /// \brief           Get the creation script for table Plugin
      /// \return          The creation script
      //--------------------------------------------------------------
      virtual std::string getTablePlugin() = 0;

      //--------------------------------------------------------------
      /// \brief           Get the creation script for table Keyword
      /// \return          The creation script
      //--------------------------------------------------------------
      virtual std::string getTableKeyword() = 0;
      
      //--------------------------------------------------------------
      /// \brief           Get the creation script for table Page
      /// \return          The creation script
      //--------------------------------------------------------------
      virtual std::string getTablePage() = 0;

      //--------------------------------------------------------------
      /// \brief           Get the creation script for table Widget
      /// \return          The creation script
      //--------------------------------------------------------------
      virtual std::string getTableWidget() = 0;
      
      //--------------------------------------------------------------
      /// \brief           Get the creation script for table PluginEventLogger
      /// \return          The creation script
      //--------------------------------------------------------------
      virtual std::string getTablePluginEventLogger() = 0;

      //--------------------------------------------------------------
      /// \brief           Get the creation script for table EventLogger
      /// \return          The creation script
      //--------------------------------------------------------------
      virtual std::string getTableEventLogger() = 0;

      //--------------------------------------------------------------
      /// \brief           Get the creation script for table Acquisition
      /// \return          The creation script
      //--------------------------------------------------------------
      virtual std::string getTableAcquisition() = 0;

      //--------------------------------------------------------------
      /// \brief           Get the creation script for table AcquisitionSummary
      /// \return          The creation script
      //--------------------------------------------------------------
      virtual std::string getTableAcquisitionSummary() = 0;

      //--------------------------------------------------------------
      /// \brief           Get the creation script for table Rule
      /// \return          The creation script
      //--------------------------------------------------------------
      virtual std::string getTableRule() = 0;

      //--------------------------------------------------------------
      /// \brief           Get the creation script for table Recipient
      /// \return          The creation script
      //--------------------------------------------------------------
      virtual std::string getTableRecipient() = 0;

      //--------------------------------------------------------------
      /// \brief           Get the creation script for table RecipientField
      /// \return          The creation script
      //--------------------------------------------------------------
      virtual std::string getTableRecipientField() = 0;

      //--------------------------------------------------------------
      /// \brief           Get the creation script for table SerialPort
      /// \return          The creation script
      //--------------------------------------------------------------
      virtual std::string getTableSerialPort() = 0;

      //--------------------------------------------------------------
      /// \brief                       Get the indexes creation scripts for table Acquisition
      /// \param [out] indexScripts    The indexes creation scripts
      //--------------------------------------------------------------
      virtual void getTableAcquisitionIndexes(std::vector<std::string> & indexScripts) = 0;


      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~ITableCreationScriptProvider()
      {
      }
   };
 
} //namespace database 

#pragma once

#include "server/database/IConfigurationRequester.h"

namespace database { namespace sqlite { 

   class CSQLiteDataProvider;
   class CSQLiteRequester;

   namespace requesters { 

      //--------------------------------------------------------------
      /// \Brief		   Configuration requester for SQLite database
      //--------------------------------------------------------------
      class CConfiguration: public IConfigurationRequester
      {
      public:
         //--------------------------------------------------------------
         /// \Brief		   Constructor
         /// \param [in]	pDatabaseHandler: the database handler
         //--------------------------------------------------------------
         CConfiguration(CSQLiteDataProvider * databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CConfiguration();

         // IConfigurationRequester implementation
         virtual void create(database::entities::CConfiguration& configurationToCreate);
         virtual boost::shared_ptr< database::entities::CConfiguration > getConfiguration(const std::string & section, const std::string & name);
         virtual std::vector< boost::shared_ptr< database::entities::CConfiguration > > getConfigurations(const std::string & section);
         virtual std::vector< boost::shared_ptr< database::entities::CConfiguration > > getConfigurations();
         virtual void updateConfiguration(database::entities::CConfiguration& configurationToUpdate);
         virtual void removeConfiguration(database::entities::CConfiguration& configurationToRemove);
         // [END] IConfigurationRequester implementation

      private:
         //--------------------------------------------------------------
         /// \Brief		   Pointer to SQLiteDatabseHandler
         //--------------------------------------------------------------
         CSQLiteDataProvider * m_databaseHandler;

         //--------------------------------------------------------------
         /// \Brief		   Reference to SQLiteRequester
         //--------------------------------------------------------------
         boost::shared_ptr<CSQLiteRequester> m_databaseRequester;
      };

   } //namespace requesters
} //namespace sqlite
} //namespace database 


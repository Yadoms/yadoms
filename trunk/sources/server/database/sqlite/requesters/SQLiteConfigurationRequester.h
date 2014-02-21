#pragma once

#include "server/database/IConfigurationRequester.h"


namespace server { 
   namespace database { 
      namespace sqlite { 
         
         class CSQLiteDataProvider;
         class CSQLiteRequester;

         namespace requesters { 

            //--------------------------------------------------------------
            /// \Brief		   Configuration requester for SQLite database
            //--------------------------------------------------------------
            class CSQLiteConfigurationRequester: public IConfigurationRequester
            {
            public:
               //--------------------------------------------------------------
               /// \Brief		   Constructor
               /// \param [in]	pDatabaseHandler: the database handler
               //--------------------------------------------------------------
               CSQLiteConfigurationRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

               //--------------------------------------------------------------
               /// \Brief		   Destructor
               //--------------------------------------------------------------
               virtual ~CSQLiteConfigurationRequester();

               // IConfigurationRequester implementation
               virtual void create(server::database::entities::CConfiguration& configurationToCreate);
               virtual boost::shared_ptr<server::database::entities::CConfiguration> getConfiguration(const std::string & section, const std::string & name);
               virtual std::vector<boost::shared_ptr<server::database::entities::CConfiguration> > getConfigurations(const std::string & section);
               virtual std::vector<boost::shared_ptr<server::database::entities::CConfiguration> > getConfigurations();
               virtual void updateConfiguration(server::database::entities::CConfiguration& configurationToUpdate);
               virtual void removeConfiguration(server::database::entities::CConfiguration& configurationToRemove);
               // [END] IConfigurationRequester implementation

            private:
               //--------------------------------------------------------------
               /// \Brief		   Reference to SQLiteDatabseHandler
               //--------------------------------------------------------------
               const CSQLiteDataProvider & m_databaseHandler;

               //--------------------------------------------------------------
               /// \Brief		   Reference to SQLiteRequester
               //--------------------------------------------------------------
               boost::shared_ptr<CSQLiteRequester> m_databaseRequester;
            };

         } //namespace requesters
      } //namespace sqlite
   } //namespace database 
} //namespace server

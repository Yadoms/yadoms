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
         CConfiguration(boost::shared_ptr<CSQLiteRequester> & databaseRequester);

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CConfiguration();

         // IConfigurationRequester implementation
         virtual void create(const entities::CConfiguration& configurationToCreate);
         virtual bool exists(const std::string & section, const std::string & name);
         virtual boost::shared_ptr<entities::CConfiguration> getConfiguration(const std::string & section, const std::string & name);
         virtual std::vector<const boost::shared_ptr<entities::CConfiguration> > getConfigurations(const std::string & section);
         virtual std::vector<const boost::shared_ptr<entities::CConfiguration> > getConfigurations();
         virtual void updateConfiguration(const entities::CConfiguration& configurationToUpdate);
         virtual void removeConfiguration(const entities::CConfiguration& configurationToRemove);
         // [END] IConfigurationRequester implementation

      private:
         //--------------------------------------------------------------
         /// \Brief		   Reference to SQLiteRequester
         //--------------------------------------------------------------
         boost::shared_ptr<CSQLiteRequester> m_databaseRequester;
      };

   } //namespace requesters
} //namespace sqlite
} //namespace database 


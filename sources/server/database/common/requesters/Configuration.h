#pragma once

#include "database/IConfigurationRequester.h"
#include "database/IDatabaseRequester.h"


namespace database { namespace common { namespace requesters { 

   //--------------------------------------------------------------
   /// \Brief		   Configuration requester for database
   //--------------------------------------------------------------
   class CConfiguration: public IConfigurationRequester
   {
   public:
      //--------------------------------------------------------------
      /// \Brief		   Constructor
      /// \param [in]	pDatabaseHandler: the database handler
      //--------------------------------------------------------------
      explicit CConfiguration(boost::shared_ptr<IDatabaseRequester> databaseRequester);

      //--------------------------------------------------------------
      /// \Brief		   Destructor
      //--------------------------------------------------------------
      virtual ~CConfiguration();

      // IConfigurationRequester implementation
      virtual void create(entities::CConfiguration& configurationToCreate);
      virtual bool exists(const std::string & section, const std::string & name);
      virtual boost::shared_ptr<entities::CConfiguration> getConfiguration(const std::string & section, const std::string & name);
      virtual std::vector<boost::shared_ptr<entities::CConfiguration> > getConfigurations(const std::string & section);
      virtual std::vector<boost::shared_ptr<entities::CConfiguration> > getConfigurations();
      virtual void updateConfiguration(entities::CConfiguration& configurationToUpdate);
      virtual void removeConfiguration(entities::CConfiguration& configurationToRemove);
      // [END] IConfigurationRequester implementation

   private:
      //--------------------------------------------------------------
      /// \Brief		   Reference to IDatabaseRequester
      //--------------------------------------------------------------
      boost::shared_ptr<IDatabaseRequester> m_databaseRequester;
   };

} //namespace requesters
} //namespace common
} //namespace database 


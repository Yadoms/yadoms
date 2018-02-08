#pragma once

#include "database/IConfigurationRequester.h"
#include "database/IDatabaseRequester.h"


namespace database
{
   namespace common
   {
      namespace requesters
      {
         //--------------------------------------------------------------
         /// \Brief		   Configuration requester for database
         //--------------------------------------------------------------
         class CConfiguration : public IConfigurationRequester
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Constructor
            /// \param [in]	databaseRequester: the database requester
            //--------------------------------------------------------------
            explicit CConfiguration(boost::shared_ptr<IDatabaseRequester> databaseRequester);

            //--------------------------------------------------------------
            /// \Brief		   Destructor
            //--------------------------------------------------------------
            virtual ~CConfiguration();

            // IConfigurationRequester implementation
            void create(entities::CConfiguration& configurationToCreate) override;
            bool exists(const std::string& section, const std::string& name) override;
            boost::shared_ptr<entities::CConfiguration> getConfiguration(const std::string& section, const std::string& name) override;
            std::vector<boost::shared_ptr<entities::CConfiguration>> getConfigurations(const std::string& section) override;
            std::vector<boost::shared_ptr<entities::CConfiguration>> getConfigurations() override;
            void updateConfiguration(entities::CConfiguration& configurationToUpdate) override;
            void removeConfigurations(const std::string& section) override;
            void removeConfiguration(entities::CConfiguration& configurationToRemove) override;
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

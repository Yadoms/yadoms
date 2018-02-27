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
            boost::shared_ptr<entities::CConfiguration> getConfiguration(const std::string& section) override;
            std::vector<boost::shared_ptr<entities::CConfiguration>> getConfigurations() override;
            void updateConfiguration(const std::string& section,
                                     const std::string& value) override;
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

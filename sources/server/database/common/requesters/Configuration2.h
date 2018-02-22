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
         class CConfiguration2 : public IConfigurationRequester
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Constructor
            /// \param [in]	databaseRequester: the database requester
            //--------------------------------------------------------------
            explicit CConfiguration2(boost::shared_ptr<IDatabaseRequester> databaseRequester);

            //--------------------------------------------------------------
            /// \Brief		   Destructor
            //--------------------------------------------------------------
            virtual ~CConfiguration2();

            // IConfigurationRequester implementation
            boost::shared_ptr<entities::CConfiguration2> getConfiguration(const std::string& section) override;
            std::vector<boost::shared_ptr<entities::CConfiguration2>> getConfigurations() override;
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

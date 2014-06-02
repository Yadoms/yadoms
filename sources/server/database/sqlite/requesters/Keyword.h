#pragma once

#include "server/database/IKeywordRequester.h"


namespace database { namespace sqlite { 

   class CSQLiteDataProvider;
   class CSQLiteRequester;

   namespace requesters { 

      //--------------------------------------------------------------
      /// \Brief		   Keyword requester for SQLite database
      //--------------------------------------------------------------
      class CKeyword : public IKeywordRequester
      {
      public:
         //--------------------------------------------------------------
         /// \Brief		   Constructor
         /// \param [in]	pDatabaseHandler: the database handler
         //--------------------------------------------------------------
         CKeyword(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CKeyword();

         // IKeywordRequester implementation
         virtual void addKeyword(boost::shared_ptr<database::entities::CKeyword> newKeyword);
         virtual boost::shared_ptr<database::entities::CKeyword> getKeyword(const int deviceId, const std::string & keyword);
         virtual std::vector<boost::shared_ptr<database::entities::CKeyword> > getKeywords(const int deviceId);
         virtual std::vector<boost::shared_ptr<database::entities::CKeyword> > getDeviceKeywordsWithCapacity(const int deviceId, const std::string & capacityName, const database::entities::ECapacityAccessMode capacityAccessMode);
         virtual void removeKeyword(const int deviceId, const std::string & keyword);
         virtual void updateKeywordFriendlyName(const int deviceId, const std::string & keyword, const std::string & newFriendlyName);
         // [END] IKeywordRequester implementation

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


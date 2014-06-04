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
         CKeyword(CSQLiteDataProvider * databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CKeyword();

         // IKeywordRequester implementation
         virtual void addKeyword(boost::shared_ptr<database::entities::CKeyword> newKeyword);
         virtual boost::shared_ptr<database::entities::CKeyword> getKeyword(const int deviceId, const std::string & keyword) const;
         virtual boost::shared_ptr<entities::CKeyword> getKeyword(const int keywordId) const;
         virtual std::vector<boost::shared_ptr<database::entities::CKeyword> > getKeywords(const int deviceId) const;
         virtual std::vector<boost::shared_ptr<database::entities::CKeyword> > getDeviceKeywordsWithCapacity(const int deviceId, const std::string & capacityName, const database::entities::ECapacityAccessMode accessMode) const;
         virtual void removeKeyword(const int deviceId, const std::string & keyword);
         virtual void removeKeyword(const int keywordId);
         virtual void updateKeywordFriendlyName(const int deviceId, const std::string & keyword, const std::string & newFriendlyName);
         virtual void updateKeywordFriendlyName(const int keywordId, const std::string & newFriendlyName);

         // [END] IKeywordRequester implementation

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


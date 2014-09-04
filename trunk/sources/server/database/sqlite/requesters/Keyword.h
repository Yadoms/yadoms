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
         virtual bool keywordExists(int deviceId, const std::string& keywordName) const;
         virtual void addKeyword(const database::entities::CKeyword& newKeyword);
         virtual boost::shared_ptr<database::entities::CKeyword> getKeyword(int deviceId, const std::string & keyword) const;
         virtual boost::shared_ptr<entities::CKeyword> getKeyword(int keywordId) const;
         virtual std::vector<boost::shared_ptr<database::entities::CKeyword> > getKeywords(int deviceId) const;
         virtual std::vector<boost::shared_ptr<database::entities::CKeyword> > getDeviceKeywordsWithCapacity(int deviceId, const std::string & capacityName, const database::entities::EKeywordAccessMode & accessMode) const;
         virtual void removeKeyword(int deviceId, const std::string & keyword);
         virtual void removeKeyword(int keywordId);
         virtual void updateKeywordFriendlyName(int deviceId, const std::string & keyword, const std::string & newFriendlyName);
         virtual void updateKeywordFriendlyName(int keywordId, const std::string & newFriendlyName);

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


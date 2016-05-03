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
         CKeyword(CSQLiteDataProvider * pDatabaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CKeyword();

         // IKeywordRequester implementation
         bool keywordExists(int deviceId,
                            const std::string& keywordName) const override;
         bool keywordExists(int keywordId) const override;
         void addKeyword(const entities::CKeyword& newKeyword) override;
         void addKeyword(int deviceId,
                         const shared::plugin::yPluginApi::historization::IHistorizable& keyword,
                         const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer) override;
         void addKeyword(int deviceId,
                         const std::string& capacityName,
                         const shared::plugin::yPluginApi::EKeywordAccessMode& accessMode,
                         const std::string& name,
                         const shared::plugin::yPluginApi::EKeywordDataType& type,
                         const std::string& units,
                         const shared::CDataContainer& typeInfo,
                         const shared::plugin::yPluginApi::historization::EMeasureType& measure,
                         const shared::CDataContainer& details) override;
         boost::shared_ptr<entities::CKeyword> getKeyword(int deviceId,
                                                          const std::string& keyword) const override;
         boost::shared_ptr<entities::CKeyword> getKeyword(int keywordId) const override;
         std::vector<boost::shared_ptr<entities::CKeyword>> getKeywordIdFromFriendlyName(int deviceId,
                                                                                         const std::string& friendlyName) const override;
         std::vector<boost::shared_ptr<entities::CKeyword>> getKeywords(int deviceId) const override;
         std::vector<boost::shared_ptr<entities::CKeyword>> getAllKeywords() const override;
         std::vector<boost::shared_ptr<entities::CKeyword>> getDeviceKeywordsWithCapacity(int deviceId,
                                                                                          const std::string& capacityName,
                                                                                          const shared::plugin::yPluginApi::EKeywordAccessMode& accessMode) const override;
         void removeKeyword(int deviceId,
                            const std::string& keyword) override;
         void removeKeyword(int keywordId) override;
         void updateKeywordFriendlyName(int deviceId,
                                        const std::string& keyword,
                                        const std::string& newFriendlyName) override;
         void updateKeywordFriendlyName(int keywordId,
                                        const std::string& newFriendlyName) override;
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


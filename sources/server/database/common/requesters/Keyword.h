#pragma once

#include "server/database/IKeywordRequester.h"
#include "server/database/IDataProvider.h"
#include "server/database/IDatabaseRequester.h"


namespace database { namespace common { namespace requesters { 

      class CAcquisition;

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
         explicit CKeyword(boost::shared_ptr<IDatabaseRequester> databaseRequester, boost::shared_ptr<CAcquisition> acquisitionRequester);

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
         void addKeywords(int deviceId,
                          const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> >& keywords) override;
         std::vector<boost::shared_ptr<entities::CKeyword> > getKeywordsMatchingCapacity(const std::string & capacity) const override;
         boost::shared_ptr<entities::CKeyword> getKeyword(int deviceId,
                                                          const std::string& keyword) const override;
         boost::shared_ptr<entities::CKeyword> getKeyword(int keywordId) const override;
         std::vector<boost::shared_ptr<entities::CKeyword> > getKeywordIdFromFriendlyName(int deviceId,
                                                                                          const std::string& friendlyName) const override;
         std::vector<boost::shared_ptr<entities::CKeyword> > getKeywords(int deviceId) const override;
         std::vector<boost::shared_ptr<entities::CKeyword> > getAllKeywords() const override;
         std::vector<boost::shared_ptr<entities::CKeyword> > getDeviceKeywordsWithCapacity(int deviceId,
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
         /// \Brief		   Pointer to acquisition requester
         //--------------------------------------------------------------
         boost::shared_ptr<common::requesters::CAcquisition> m_acquisitionRequester;

         //--------------------------------------------------------------
         /// \Brief		   Reference to IDatabaseRequester
         //--------------------------------------------------------------
         boost::shared_ptr<IDatabaseRequester> m_databaseRequester;
      };

} //namespace requesters
} //namespace common
} //namespace database 


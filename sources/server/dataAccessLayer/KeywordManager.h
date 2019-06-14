#pragma once
#include "IKeywordManager.h"
#include "database/IDataProvider.h"

namespace dataAccessLayer
{
   class CKeywordManager : public IKeywordManager
   {
   public:
      explicit CKeywordManager(boost::shared_ptr<database::IDataProvider> dataProvider);

      virtual ~CKeywordManager() = default;

      // IKeywordManager implementation
      bool keywordExists(int deviceId, const std::string& keywordName) const override;
      bool keywordExists(int keywordId) const override;
      boost::shared_ptr<database::entities::CKeyword> getKeyword(int deviceId, const std::string& keyword) const override;
      boost::shared_ptr<database::entities::CKeyword> getKeyword(int keywordId) const override;
      std::vector<boost::shared_ptr<database::entities::CKeyword>> getKeywordIdFromFriendlyName(int deviceId, const std::string& friendlyName) const override;
      std::vector<boost::shared_ptr<database::entities::CKeyword>> getAllKeywords() const override;
      std::vector<boost::shared_ptr<database::entities::CKeyword>> getKeywords(int deviceId) const override;
      std::vector<boost::shared_ptr<database::entities::CKeyword>> getKeywordsMatchingCapacity(const std::string& capacity) const override;
      std::vector<boost::shared_ptr<database::entities::CKeyword>> getDeviceKeywordsWithCapacity( int deviceId, const std::string& capacityName, const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode) const override;
      boost::shared_ptr<database::entities::CAcquisition> getKeywordLastAcquisition(const int keywordId, bool throwIfNotExists = true) override;
      std::string getKeywordLastData(const int keywordId, bool throwIfNotExists = true) override;
      std::vector<boost::tuple<int, std::string>> getKeywordListLastData(const std::vector<int> keywordIds) override;
      void addKeyword(int deviceId, const shared::plugin::yPluginApi::historization::IHistorizable& keyword, const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer) override;
      void addKeywords(int deviceId, const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>& keywords) override;
      void updateKeywordFriendlyName(int deviceId, const std::string& keyword, const std::string& newFriendlyName) override;
      void updateKeywordFriendlyName(int keywordId, const std::string& newFriendlyName) override;
      void updateKeywordBlacklistState(int keywordId, const bool blacklist) override;
      void removeKeyword(int deviceId, const std::string& keyword) override;
      void removeKeyword(int keywordId) override;
      // [END] IKeywordManager implementation

   private:
      void addKeyword(const database::entities::CKeyword& newKeyword) const;
      static boost::shared_ptr<database::entities::CKeyword> makeKeywordEntity(int deviceId,
                                                                               const shared::plugin::yPluginApi::historization::IHistorizable&
                                                                               keyword,
                                                                               const shared::CDataContainer& details = shared::CDataContainer::
                                                                                  EmptyContainer);


      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      boost::shared_ptr<database::IKeywordRequester> m_keywordRequester;
   };
} //namespace dataAccessLayer 

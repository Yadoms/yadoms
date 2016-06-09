#pragma once
#include "server/database/IDataProvider.h"
#include "server/database/IDatabaseRequester.h"
#include "server/database/IKeywordRequester.h"

namespace database
{
   namespace common
   {
      namespace requesters
      {
         class CAcquisition;

         //--------------------------------------------------------------
         /// \Brief		   Keyword requester for SQLite database
         //--------------------------------------------------------------
         class CKeyword : public IKeywordRequester
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Constructor
            /// \param [in]	databaseRequester    The database requester
            /// \param [in]	acquisitionRequester The acquisition requester
            //--------------------------------------------------------------
            CKeyword(boost::shared_ptr<IDatabaseRequester> databaseRequester,
                     boost::shared_ptr<CAcquisition> acquisitionRequester);

            //--------------------------------------------------------------
            /// \Brief		   Destructor
            //--------------------------------------------------------------
            virtual ~CKeyword();

            // IKeywordRequester implementation
            void addKeyword(const entities::CKeyword& newKeyword) override;
            std::vector<boost::shared_ptr<entities::CKeyword> > getKeywordsMatchingCapacity(const std::string& capacity) const override;
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
            void removeKeyword(int keywordId) override;
            void updateKeywordFriendlyName(int keywordId,
                                           const std::string& newFriendlyName) override;
            // [END] IKeywordRequester implementation

         private:
            //--------------------------------------------------------------
            /// \Brief		   The database requester
            //--------------------------------------------------------------
            boost::shared_ptr<IDatabaseRequester> m_databaseRequester;

            //--------------------------------------------------------------
            /// \Brief		   The acquisition requester
            //--------------------------------------------------------------
            boost::shared_ptr<CAcquisition> m_acquisitionRequester;
         };
      } //namespace requesters
   } //namespace common
} //namespace database 



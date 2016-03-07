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
         virtual bool keywordExists(int deviceId, const std::string& keywordName) const;
         virtual bool keywordExists(int keywordId) const;
         virtual void addKeyword(const entities::CKeyword& newKeyword);
         virtual boost::shared_ptr<entities::CKeyword> getKeyword(int deviceId, const std::string & keyword) const;
         virtual boost::shared_ptr<entities::CKeyword> getKeyword(int keywordId) const;
         virtual std::vector<boost::shared_ptr<entities::CKeyword> > getKeywordIdFromFriendlyName(int deviceId, const std::string & friendlyName) const;
         virtual std::vector<boost::shared_ptr<entities::CKeyword> > getKeywords(int deviceId) const;
         virtual std::vector<boost::shared_ptr<entities::CKeyword> > getAllKeywords() const;
         virtual std::vector<boost::shared_ptr<entities::CKeyword> > getDeviceKeywordsWithCapacity(int deviceId, const std::string & capacityName, const shared::plugin::yPluginApi::EKeywordAccessMode & accessMode) const;
         virtual void removeKeyword(int deviceId, const std::string & keyword);
         virtual void removeKeyword(int keywordId);
         virtual void updateKeywordFriendlyName(int deviceId, const std::string & keyword, const std::string & newFriendlyName);
         virtual void updateKeywordFriendlyName(int keywordId, const std::string & newFriendlyName);

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

